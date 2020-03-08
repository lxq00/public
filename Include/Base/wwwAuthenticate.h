#pragma  once
#include "Base/IntTypes.h"

namespace Public {
namespace Base {

#define HTTPHEADER_WWWAUTHENTICATE "WWW-Authenticate"
#define HTTPHEADER_AUTHENTICATE "Authorization"

struct BASE_API WWW_Authenticate
{
public:
	typedef enum {
		Authenticate_Type_Basic,
		Authenticate_Type_Digest,
	}Authenticate_Type;
public:
	//�����֤��Ϣ
	static bool checkAuthenticate(const std::string& method, const std::string& username, const std::string& password, const std::string& pAuthorization);

	//����һ����֤������Ϣ
	static std::string buildWWWAuthenticate(const std::string& method, const std::string& username, const std::string& password, Authenticate_Type type);

	//����һ����֤�ظ���Ϣ
	static std::string buildAuthorization(const std::string& method, const std::string& username, const std::string& password, const std::string& uri, const std::string& wwwauthen);

	static std::string getAuthorizationUsreName(const std::string& wwwauthen);
};

}
}
