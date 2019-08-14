#pragma once

#include "push-pull.h"

class Exchange:public Strand
{
	struct ExChangeStrandData;
	typedef Function<RedisValue, ExChangeStrandData*, const std::vector<RedisValue> &> MessageCommandCallback;

	struct ExChangeStrandData :public StrandData
	{
		void*					user;
		shared_ptr<RedisValue>	value;
		MessageCommandCallback	callback;
		CmdResultCallback		ackcallback;
	};
	struct ExchangeInfo
	{
		enum _ExChangeType
		{
			ExChangeType_SubToPush,
			ExChangeType_PullToPublish,
		}type;

		std::string srcChannel;
		std::string dstChannel;

		ExchangeInfo(_ExChangeType _type,const std::string& src,const std::string& dst)
			:type(_type), srcChannel(src), dstChannel(dst){}

		~ExchangeInfo() {}
	};
public:
	Exchange(const shared_ptr<IOWorker>& worker) :Strand(worker)
	{
		subCommand("SUBSCRIBE", MessageCommandCallback(&Exchange::subscribeFunc, this));
		subCommand("UNSUBSCRIBE", MessageCommandCallback(&Exchange::unsubscribeFunc, this));
		subCommand("PUBLISH", MessageCommandCallback(&Exchange::publishFunc, this));
		subCommand("PULL", MessageCommandCallback(&Exchange::pullFunc, this));
		subCommand("UNPULL", MessageCommandCallback(&Exchange::unpullFunc, this));
		subCommand("PUSH", MessageCommandCallback(&Exchange::pushFunc, this));
		subCommand("EXCHANGE", MessageCommandCallback(&Exchange::exchangeFunc, this));
		subCommand("UNEXCHANGE", MessageCommandCallback(&Exchange::unexchangeFunc, this));
		subCommand("EXCHANGEINFO", MessageCommandCallback(&Exchange::exchangeinfoFunc, this));
	}
	~Exchange() {}
	bool inputCommand(const CmdResultCallback& callback, void* user, const shared_ptr<RedisValue>& value)
	{
		if (!value->isArray()) return false;

		const std::vector<RedisValue> & valuearray = value->getArray();

		std::string cmd = String::tolower(valuearray[0].toString());
		std::map<std::string, MessageCommandCallback>::iterator iter = callbacklist.find(cmd);
		if (iter == callbacklist.end())
		{
			return false;
		}

		shared_ptr<ExChangeStrandData> data = make_shared<ExChangeStrandData>();
		data->user = user;
		data->callback = iter->second;
		data->value = value;
		data->ackcallback = callback;

		post(Strand::StrandCallback(&Exchange::strandCallback, this), data);

		return true;
	}
	void userOffline(void* user)
	{
		shared_ptr<ExChangeStrandData> data = make_shared<ExChangeStrandData>();
		data->user = user;

		post(Strand::StrandCallback(&Exchange::strandUserCallback, this), data);
	}
private:
	RedisValue subscribeFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if(val.size() < 2) return RedisValue(false, "wrong number of arguments");

		std::vector<RedisValue> ackmsgarray;
		for (uint32_t i = 1; i < val.size(); i++)
		{
			std::string channel = String::tolower(val[i].toString());

			std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.find(channel);
			if (iter == pubsublist.end())
			{
				shared_ptr<Pub_Sub> pubsub = make_shared<Pub_Sub>();
				pubsublist[channel] = pubsub;
				iter = pubsublist.find(channel);
			}
			iter->second->subscribe(data->user,data->ackcallback);

			std::vector<RedisValue> ackmsg;
			ackmsg.push_back(RedisValue("subscribe"));
			ackmsg.push_back(RedisValue(channel));
			ackmsg.push_back(RedisValue(i));

			ackmsgarray.push_back(ackmsg);
		}

		return RedisValue(ackmsgarray);
	}
	RedisValue unsubscribeFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 2) return RedisValue(false, "wrong number of arguments");
		std::string channel = String::tolower(val[1].toString());

		bool unsubflag = false;

		std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.find(channel);
		if (iter != pubsublist.end())
		{
			unsubflag = iter->second->unsubscribe(data->user);

			if (iter->second->subscribesize() == 0)
			{
				pubsublist.erase(iter);
			}
		}

		return RedisValue(unsubflag ? 1 : 0);
	}
	RedisValue publishFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 3) return RedisValue(false,"wrong number of arguments");

		std::string channel = String::tolower(val[1].toString());
		const RedisValue& msg = val[2].toString();

		uint32_t subsize = 0;
		std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.find(channel);
		if (iter != pubsublist.end())
		{
			RedisValue notifymsg;
			{
				std::vector<RedisValue> msglist;
				msglist.push_back(RedisValue("message"));
				msglist.push_back(channel);
				msglist.push_back(msg);
				notifymsg = RedisValue(msglist);
			}

			iter->second->publish(notifymsg);
			subsize = iter->second->subscribesize();
		}

		return RedisValue(subsize);
	}
	RedisValue pullFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() < 2) return RedisValue(false, "wrong number of arguments");

		std::vector<RedisValue> ackmsgarray;
		for (uint32_t i = 1; i < val.size(); i++)
		{
			std::string channel = String::tolower(val[i].toString());

			std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.find(channel);
			if (iter == pushpulllist.end())
			{
				shared_ptr<Push_Pull> pubsub = make_shared<Push_Pull>();
				pushpulllist[channel] = pubsub;
				iter = pushpulllist.find(channel);
			}
			iter->second->pull(data->user,data->ackcallback);

			std::vector<RedisValue> ackmsg;
			ackmsg.push_back(RedisValue("pull"));
			ackmsg.push_back(RedisValue(channel));
			ackmsg.push_back(RedisValue(i));

			ackmsgarray.push_back(ackmsg);
		}

		return RedisValue(ackmsgarray);
	}
	RedisValue unpullFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 2) return RedisValue(false, "wrong number of arguments");
		std::string channel = String::tolower(val[1].toString());

		bool unsubflag = false;

		std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.find(channel);
		if (iter != pushpulllist.end())
		{
			unsubflag = iter->second->unpull(data->user);

			if (iter->second->pullsize() == 0)
			{
				pushpulllist.erase(iter);
			}
		}

		return RedisValue(unsubflag ? 1 : 0);
	}
	RedisValue pushFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 3) return RedisValue(false, "wrong number of arguments");

		std::string channel = String::tolower(val[1].toString());
		const RedisValue& msg = val[2].toString();

		uint32_t pullsize = 0;
		std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.find(channel);
		if (iter != pushpulllist.end())
		{
			RedisValue notifymsg;
			{
				std::vector<RedisValue> msglist;
				msglist.push_back(RedisValue("message"));
				msglist.push_back(channel);
				msglist.push_back(msg);
				notifymsg = RedisValue(msglist);
			}

			iter->second->push(notifymsg);
			pullsize = iter->second->pullsize();
		}

		return RedisValue(pullsize);
	}
	RedisValue exchangeFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 5) return RedisValue(false, "wrong number of arguments");

		std::string srctype = String::tolower(val[1].toString());
		std::string srcchannel = String::tolower(val[2].toString());
		std::string dsttype = String::tolower(val[3].toString());
		std::string dstchannel = String::tolower(val[4].toString());

		ExchangeInfo::_ExChangeType exchangetype;

		if (srctype == "subscribe" && dsttype == "push")
		{
			exchangetype = ExchangeInfo::ExChangeType_SubToPush;
		}
		else if(srctype == "pull" && dsttype == "publish")
		{
			exchangetype = ExchangeInfo::ExChangeType_PullToPublish;
		}
		else
		{
			return RedisValue(false, "syntax error");
		}
		
		for (std::list<shared_ptr<ExchangeInfo> >::iterator iter = exchangelist.begin(); iter != exchangelist.end(); iter++)
		{
			if (exchangetype == (*iter)->type && srcchannel == (*iter)->srcChannel && dstchannel == (*iter)->dstChannel)
			{
				return RedisValue(1);
			}
			else if (exchangetype != (*iter)->type && srcchannel == (*iter)->srcChannel && dstchannel == (*iter)->dstChannel)
			{
				return RedisValue(false,"exchange loop");
			}
		}

		shared_ptr<ExchangeInfo> exchangeinfo = make_shared<ExchangeInfo>(exchangetype,srcchannel,dstchannel);
		if (exchangetype == ExchangeInfo::ExChangeType_SubToPush)
		{
			std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.find(srcchannel);
			if (iter == pubsublist.end())
			{
				shared_ptr<Pub_Sub> pubsub = make_shared<Pub_Sub>();
				pubsublist[srcchannel] = pubsub;
				iter = pubsublist.find(srcchannel);
			}
			iter->second->subscribe(exchangeinfo.get(), CmdMessageCallback(&Exchange::exchangeCallback, this));
		}
		else if (exchangetype == ExchangeInfo::ExChangeType_PullToPublish)
		{
			std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.find(srcchannel);
			if (iter == pushpulllist.end())
			{
				shared_ptr<Push_Pull> pushpull = make_shared<Push_Pull>();
				pushpulllist[srcchannel] = pushpull;
				iter = pushpulllist.find(srcchannel);
			}
			iter->second->pull(exchangeinfo.get(), CmdMessageCallback(&Exchange::exchangeCallback, this));
		}

		exchangelist.push_back(exchangeinfo);

		return RedisValue(1);
	}
	RedisValue unexchangeFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		if (val.size() != 5) return RedisValue(false, "wrong number of arguments");

		std::string srctype = String::tolower(val[1].toString());
		std::string srcchannel = String::tolower(val[2].toString());
		std::string dsttype = String::tolower(val[3].toString());
		std::string dstchannel = String::tolower(val[4].toString());

		ExchangeInfo::_ExChangeType exchangetype;

		if (srctype == "subscribe" && dsttype == "push")
		{
			exchangetype = ExchangeInfo::ExChangeType_SubToPush;
		}
		else if (srctype == "pull" && dsttype == "publish")
		{
			exchangetype = ExchangeInfo::ExChangeType_PullToPublish;
		}
		else
		{
			return RedisValue(false, "syntax error");
		}

		for (std::list<shared_ptr<ExchangeInfo> >::iterator iter = exchangelist.begin(); iter != exchangelist.end(); iter++)
		{
			if (exchangetype == (*iter)->type && srcchannel == (*iter)->srcChannel && dstchannel == (*iter)->dstChannel)
			{
				if (exchangetype == ExchangeInfo::ExChangeType_SubToPush)
				{
					std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.find(srcchannel);
					if (iter != pubsublist.end())
					{
						iter->second->unsubscribe(iter->second.get());
						if (iter->second->subscribesize() == 0)
						{
							pubsublist.erase(iter);
						}
					}
				}
				else if (exchangetype == ExchangeInfo::ExChangeType_PullToPublish)
				{
					std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.find(srcchannel);
					if (iter != pushpulllist.end())
					{
						iter->second->unpull(iter->second.get());

						if (iter->second->pullsize() == 0)
						{
							pushpulllist.erase(iter);
						}
					}
				}

				exchangelist.erase(iter);
				break;
			}
		}

		return RedisValue(1);
	}
	RedisValue exchangeinfoFunc(ExChangeStrandData* data, const std::vector<RedisValue> & val)
	{
		std::vector<RedisValue> redisarray;

		for (std::list<shared_ptr<ExchangeInfo> >::iterator iter = exchangelist.begin(); iter != exchangelist.end(); iter++)
		{
			std::vector<RedisValue> exchangeinfo;
			if ((*iter)->type == ExchangeInfo::ExChangeType_PullToPublish)
			{
				exchangeinfo.push_back(RedisValue("From Pull To Publish"));
			}
			else
			{
				exchangeinfo.push_back(RedisValue("From Subcribe To Push"));
			}
			exchangeinfo.push_back(RedisValue("From:" + (*iter)->srcChannel));
			exchangeinfo.push_back(RedisValue("To:" + (*iter)->dstChannel));

			redisarray.push_back(RedisValue(exchangeinfo));
		}

		return RedisValue(redisarray);
	}
private:
	void subCommand(const std::string& cmd, const MessageCommandCallback& callback)
	{
		std::string subcmd = String::tolower(cmd);
		callbacklist[subcmd] = callback;
	}
	void strandUserCallback(const shared_ptr<StrandData>& data)
	{
		shared_ptr<StrandData> tmp = data;
		if (tmp == NULL) return;

		ExChangeStrandData* stranddata = (ExChangeStrandData*)tmp.get();

		for (std::map<std::string, shared_ptr<Pub_Sub> >::iterator iter = pubsublist.begin(); iter != pubsublist.end(); iter++)
		{
			iter->second->unsubscribe(stranddata->user);
		}
		for (std::map<std::string, shared_ptr<Push_Pull> >::iterator iter = pushpulllist.begin(); iter != pushpulllist.end(); iter++)
		{
			iter->second->unpull(stranddata->user);
		}
	}
	void strandCallback(const shared_ptr<StrandData>& data)
	{
		shared_ptr<StrandData> tmp = data;
		if (tmp == NULL) return;

		ExChangeStrandData* stranddata = (ExChangeStrandData*)tmp.get();
		const std::vector<RedisValue> & valuearray = stranddata->value->getArray();

		RedisValue ret = stranddata->callback(stranddata, valuearray);
		if (!ret.isNull())
		{
			stranddata->ackcallback(stranddata->user, ret);
		}
	}
	void exchangeCallback(void*, const RedisValue& val)
	{
		
	}
private:
	std::map<std::string, MessageCommandCallback>	callbacklist;
	std::map<std::string, shared_ptr<Pub_Sub> >	 pubsublist;
	std::map<std::string, shared_ptr<Push_Pull> > pushpulllist;
	std::list<shared_ptr<ExchangeInfo> > exchangelist;
};