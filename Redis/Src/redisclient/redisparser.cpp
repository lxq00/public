#ifndef REDISCLIENT_REDISPARSER_CPP
#define REDISCLIENT_REDISPARSER_CPP

#include "redisparser.h"

namespace redisclient {

RedisParser::RedisParser(): bulkSize(0)
{
    buf.reserve(64);
}
RedisParser::~RedisParser() {}
bool RedisParser::input(const char *ptr, size_t size)
{
	size_t position = 0;
    State state = Start;

    if (!states.empty())
    {
        state = states.top();
        states.pop();
    }

    while(position < size)
    {
        char c = ptr[position++];
        switch(state)
        {
            case StartArray:
            case Start:
                buf.clear();
                switch(c)
                {
                    case stringReply:
                        state = String;
                        break;
                    case errorReply:
                        state = ErrorString;
                        break;
                    case integerReply:
                        state = Integer;
                        break;
                    case bulkReply:
                        state = BulkSize;
                        bulkSize = 0;
                        break;
                    case arrayReply:
                        state = ArraySize;
                        break;
                    default:
						logdebug("RedisParser::input error");
						return false;
                }
                break;
            case String:
                if( c == '\r' )
                {
                    state = StringLF;
                }
                else if( isChar(c) && !isControl(c) )
                {
                    buf.push_back(c);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case ErrorString:
                if( c == '\r' )
                {
                    state = ErrorLF;
                }
                else if( isChar(c) && !isControl(c) )
                {
                    buf.push_back(c);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case BulkSize:
                if( c == '\r' )
                {
                    if( buf.empty() )
                    {
						logdebug("RedisParser::input error");
						return false;
                    }
                    else
                    {
                        state = BulkSizeLF;
                    }
                }
                else if( isdigit(c) || c == '-' )
                {
                    buf.push_back(c);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case StringLF:
                if( c == '\n')
                {
                    state = Start;
                    redisValue = RedisValue(buf);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case ErrorLF:
                if( c == '\n')
                {
                    state = Start;
                    RedisValue::ErrorTag tag;
                    redisValue = RedisValue(buf, tag);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case BulkSizeLF:
                if( c == '\n' )
                {
                    bulkSize = bufToLong(buf.data(), buf.size());
                    buf.clear();

                    if( bulkSize == -1 )
                    {
                        state = Start;
                        redisValue = RedisValue(); // Nil
                    }
                    else if( bulkSize == 0 )
                    {
                        state = BulkCR;
                    }
                    else if( bulkSize < 0 )
                    {
						logdebug("RedisParser::input error");
						return false;
                    }
                    else
                    {
                        buf.reserve(bulkSize);

                        long int available = size - position;
                        long int canRead = min(bulkSize, available);

                        if( canRead > 0 )
                        {
                            buf.assign(ptr + position, ptr + position + canRead);
                            position += canRead;
                            bulkSize -= canRead;
                        }


                        if (bulkSize > 0)
                        {
                            state = Bulk;
                        }
                        else
                        {
                            state = BulkCR;
                        }
                    }
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case Bulk: {
                //assert( bulkSize > 0 );

                long int available = size - position + 1;
                long int canRead = min(available, bulkSize);

                buf.insert(buf.end(), ptr + position - 1, ptr + position - 1 + canRead);
                bulkSize -= canRead;
                position += canRead - 1;

                if( bulkSize == 0 )
                {
                    state = BulkCR;
                }
                break;
            }
            case BulkCR:
                if( c == '\r')
                {
                    state = BulkLF;
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case BulkLF:
                if( c == '\n')
                {
                    state = Start;
                    redisValue = RedisValue(buf);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case ArraySize:
                if( c == '\r' )
                {
                    if( buf.empty() )
                    {
						logdebug("RedisParser::input error");
						return false;
					}
                    else
                    {
                        state = ArraySizeLF;
                    }
                }
                else if( isdigit(c) || c == '-' )
                {
                    buf.push_back(c);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case ArraySizeLF:
                if( c == '\n' )
                {
                    int64_t arraySize = bufToLong(buf.data(), buf.size());
                    std::vector<RedisValue> array;

                    if( arraySize == -1 )
                    {
                        state = Start;
                        redisValue = RedisValue();  // Nil value
                    }
                    else if( arraySize == 0 )
                    {
                        state = Start;
                        redisValue = RedisValue(std::move(array));  // Empty array
                    }
                    else if( arraySize < 0 )
                    {
						logdebug("RedisParser::input error");
						return false;
                    }
                    else
                    {
                        array.reserve((int)arraySize);
                        arraySizes.push((long)arraySize);
                        arrayValues.push(std::move(array));

                        state = StartArray;
                    }
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case Integer:
                if( c == '\r' )
                {
                    if( buf.empty() )
                    {
						logdebug("RedisParser::input error");
						return false;
                    }
                    else
                    {
                        state = IntegerLF;
                    }
                }
                else if( isdigit(c) || c == '-' )
                {
                    buf.push_back(c);
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            case IntegerLF:
                if( c == '\n' )
                {
                    int64_t value = bufToLong(buf.data(), buf.size());

                    buf.clear();
                    redisValue = RedisValue(value);
                    state = Start;
                }
                else
                {
					logdebug("RedisParser::input error");
					return false;
                }
                break;
            default:
				logdebug("RedisParser::input error");
				return false;
        }


        if (state == Start)
        {
            if (!arraySizes.empty())
            {
                //assert(arraySizes.size() > 0);
                arrayValues.top().getArray().push_back(redisValue);

                while(!arraySizes.empty() && --arraySizes.top() == 0)
                {
                    arraySizes.pop();
                    redisValue = std::move(arrayValues.top());
                    arrayValues.pop();

                    if (!arraySizes.empty())
                        arrayValues.top().getArray().push_back(redisValue);
                }
            }


            if (arraySizes.empty())
            {
				valueList.push_back(std::move(redisValue));
            }
        }
    }

	if (arraySizes.empty() && state == Start)
	{
	}
	else
	{
		states.push(state);
    }

	return true;
}

shared_ptr<RedisValue> RedisParser::result()
{
	if (valueList.size() <= 0) return shared_ptr<RedisValue>();

	shared_ptr<RedisValue> val = make_shared<RedisValue>(std::move(valueList.front()));
	valueList.pop_front();

    return val;
}

/*
 * Convert string to long. I can't use atol/strtol because it
 * work only with null terminated string. I can use temporary
 * std::string object but that is slower then bufToLong.
 */
long int RedisParser::bufToLong(const char *str, size_t size)
{
    long int value = 0;
    bool sign = false;

    if( str == nullptr || size == 0 )
    {
        return 0;
    }

    if( *str == '-' )
    {
        sign = true;
        ++str;
        --size;

        if( size == 0 ) {
            return 0;
        }
    }

    for(const char *end = str + size; str != end; ++str)
    {
        char c = *str;

        // char must be valid, already checked in the parser
        //assert(c >= '0' && c <= '9');

        value = value * 10;
        value += c - '0';
    }

    return sign ? -value : value;
}


#define crlf "\r\n"

void RedisBuilder::bufferAppend(std::string &vec, char buf)
{
	vec += buf;
}
void RedisBuilder::bufferAppend(std::string &vec, const char* buf)
{
	vec += buf;
}
void RedisBuilder::bufferAppend(std::string &vec,const std::string &val)
{
	vec += val;
}

std::string RedisBuilder::makeCommand(const std::deque<Value> &items)
{
	std::string cmdstr;

	bufferAppend(cmdstr, '*');
	bufferAppend(cmdstr, std::to_string(items.size()));
	bufferAppend(cmdstr, crlf);

	for (const auto &item : items)
	{
		std::string tmpstr = item.readString();

		bufferAppend(cmdstr, '$');
		bufferAppend(cmdstr, std::to_string(tmpstr.size()));
		bufferAppend(cmdstr, crlf);
		bufferAppend(cmdstr, tmpstr);
		bufferAppend(cmdstr, crlf);
	}

	return std::move(cmdstr);
}

}

#endif // REDISCLIENT_REDISPARSER_CPP
