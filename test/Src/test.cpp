#if 0
#include "Base/Func.h"
using namespace Public::Base;
#include <functional>
#include <memory>
#include "HTTP/HTTPClient.h"
typedef void(*ptrtype)(int);

struct Test
{
	void testfunc(int) {}
};

template <typename Function>
struct function_traits : public function_traits < decltype(&Function::operator()) >
{

};

template <typename ClassType, typename ReturnType, typename Args, typename Args2>
struct function_traits < ReturnType(ClassType::*)(Args, Args2) const >
{
	typedef ReturnType(*pointer)(Args, Args2);
	typedef std::function<ReturnType(Args, Args2)> function;
};
#include "boost/regex.hpp"
int main()
{
	std::string url = "/api/entities/11";
	std::string math = "^/api/entities/.+";

	boost::regex  regex (math);

	if (boost::regex_match(url, regex))
	{
		int a = 0;
	}
	else
	{
		int b = 0;
	}

	//Public::HTTP::HTTPClient client("http://192.168.0.11");
	//shared_ptr<Public::HTTP::HTTPResponse> respse = client.request("get");

	int a = 0;


	/*ifstream infile;
	infile.open("test.vcxproj.user", std::ofstream::in | std::ofstream::binary);
	if (!infile.is_open()) return false;

	while (!infile.eof())
	{
		char buffer[1024];
		streamsize readlen = infile.readsome(buffer, 1024);
		if (readlen == 0) break;
	}


	infile.close();*/


	//std::shared_ptr<Test> testptr(new Test);

	//Function1<void, int> testfunc = Function1<void, int>(&Test::testfunc, testptr.get());

	//test(1);
	
	//Function2<void,int, int> f = std::bind(&Test::testfunc, std::weak_ptr<Test>(t).lock(), std::placeholders::_1, std::placeholders::_2);

	//Function1<void, int> f1 = [&](int) {
	//	int a = 0;
	//};

	//std::function<void(int)> test1 = f1;

	/*ptrtype ptrtmp = test;

	if (test == test1)
	{
		int a = 0;
	}
	if (test != test1)
	{
		int b = 0;
	}*/
	//std::map < std::string, Host::NetworkInfo > infos;
	//std::string defaultMac;

	//Host::getNetworkInfos(infos, defaultMac);

	return 0;
}
#endif

#if 0
#include "Medis/Medis.h"
using namespace Public::Medis;

#define WORKPORT 6379

shared_ptr<IOWorker>		worker;
shared_ptr<Service>			service;


void systemExit(void*, BaseSystem::CloseEvent)
{
	service = NULL;
	worker = NULL;


	Public::Network::NetworkSystem::uninit();
	Public::Base::BaseSystem::uninit();

	Public::Base::BaseSystem::autoExitDelayer(30);
}
int main(int argc, const char* argv[])
{
	Public::Base::BaseSystem::init(systemExit);
	Public::Network::NetworkSystem::init();


	worker = make_shared<IOWorker>(16);
	service = make_shared<Service>();

	if (!service->start(worker, WORKPORT))
	{
		int a = 0;
		return -1;
	}

	ConsoleCommand cmd;
	cmd.run("medis");

	return 0;
}
#endif

#if 0
#include "OnvifClient/OnvifClient.h"
using namespace Public::Onvif;

int main()
{
	shared_ptr<IOWorker> worker = make_shared<IOWorker>(4);

	shared_ptr<OnvifClientManager> manager = make_shared<OnvifClientManager>(worker,"test");

	shared_ptr<OnvifClient> client = manager->create(URL("admin:admin@192.168.13.33"));

//-	shared_ptr<OnvifClientDefs::Info> info = client->getInfo();

	shared_ptr<OnvifClientDefs::Capabilities> cap = client->getCapabities();	//��ȡ�豸�������ϣ�������Ϣʹ��XM_GetLastError����

	//shared_ptr<OnvifClientDefs::Scopes> scopes = client->getScopes(); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����


//-	shared_ptr<OnvifClientDefs::Profiles> profile = client->getProfiles(); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����
//	std::string getStreamUrl(const std::string& streamtoken, int timeoutms = 10000); //��ȡ����Ϣ,������Ϣʹ��XM_GetLastError����
//	std::string getSnapUrl(const std::string& snaptoken, int timeoutms = 10000);	//��ȡ��ͼ��Ϣ��������Ϣʹ��XM_GetLastError����

//	shared_ptr<OnvifClientDefs::NetworkInterfaces> network = client->getNetworkInterfaces();//������Ϣ��������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::VideoEncoderConfigurations> enc = client->getVideoEncoderConfigurations(); //��ȡ��Ƶ������Ϣ��������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::ContinuousMove> move = client->getContinuousMove(); //������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::AbsoluteMove> abs = client->getAbsoluteMove(); //������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::_PTZConfig> config = client->getConfigurations(); //������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::ConfigurationOptions> opt = client->getConfigurationOptions(); //������Ϣʹ��XM_GetLastError����
//	shared_ptr<Time> time = client->GetSystemDatetime(); //������Ϣʹ��XM_GetLastError����

	shared_ptr<OnvifClientDefs::StartRecvAlarm> alarminfo = client->startRecvAlarm(cap);

	client->recvAlarm(alarminfo);

	getchar();

	return 0;
}
#endif

#if 0
#include "Base/Base.h"

using namespace Public::Base;

#define MAXBUFFERLEN		1024

int main()
{

	FILE* fd = fopen("test.md","wb+");

	char* buffer = new char [MAXBUFFERLEN];

	uint64_t startime = Time::getCurrentMilliSecond();
	uint64_t writetotlsize = 0;
	uint64_t	prevtime = startime;
	while (1)
	{
		size_t writelen = fwrite(buffer, 1, MAXBUFFERLEN, fd);
		if (writelen <= 0) break;

		fflush(fd);

		writetotlsize += MAXBUFFERLEN;

		uint64_t nowtime = Time::getCurrentMilliSecond();

		if (nowtime - prevtime >= 1000)
		{
			logdebug("write speed %llu",writetotlsize * 1000 / (nowtime - startime));

			prevtime = nowtime;
		}
	}

	fclose(fd);

	getchar();
}

#endif

#if 0
#include "Base/Base.h"
using namespace Public::Base;


class AClass
{
public:
	virtual void testa() {}
	virtual void testb() {}
};

class BClass :public AClass
{
public:

	virtual void testa() {}
	virtual void testc() {}
};

int main()
{
	AClass* ptr = new BClass;



	auto val = typeid(ptr).raw_name();


	getchar();

	return 0;
}

#endif




#if 1
#include "Base/Base.h"
using namespace Public::Base;

extern int runClient(const std::string& ipaddr, const std::list<std::string>& rtsplist);
extern int runserver(const std::list<std::string>& rtsplist);


int main(int argc, char** argv)
{

	std::list<std::string> rtspaddrlist;
	{
		std::string filename = File::getExcutableFileFullPath() + "/rtsplist.ini";
		
		FILE* fd = fopen(filename.c_str(), "rb");
		if (fd == NULL)
		{
			logerror("load config %s err",filename.c_str());
			getchar();

			return -1;
		}

		char addr[256];
		while (fgets(addr, 255, fd))
		{
			int len = strlen(addr);
			while (len > 0)
			{
				if (addr[len - 1] == '\r' || addr[len - 1] == '\n')
				{
					addr[len - 1] = 0;
					len -= 1;
				}
				else
				{
					break;
				}
			}

			rtspaddrlist.push_back(addr);
		}
		fclose(fd);
	}

	if (rtspaddrlist.size() == 0)
	{
		logerror("not rtsp addr");
		getchar();

		return -2;
	}

	{
		std::string tranrtspaddrlist = File::getExcutableFileFullPath() + "/rtsplist_trans.ini";
		FILE* fd = fopen(tranrtspaddrlist.c_str(), "wb+");

		if (fd != NULL)
		{
			std::list<std::string> rtsplisttmp;
			for (std::list<std::string>::const_iterator iter = rtspaddrlist.begin(); iter != rtspaddrlist.end(); iter++)
			{
				std::string rtspaddr = "rtsp://127.0.0.1:5554/" + Base64::encode(*iter)+"\r\n";

				fwrite(rtspaddr.c_str(), 1, rtspaddr.length(), fd);
			}

			fclose(fd);
		}
	}

	if (argc == 1)
	{
		runserver(rtspaddrlist);
	}
	else
	{
		runClient(argv[1], rtspaddrlist);
	}

	
	return 0;
}
#endif