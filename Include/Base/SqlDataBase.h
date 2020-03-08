#pragma once

#include "Base/Defs.h"
#include "Base/IntTypes.h"
#include "Base/Time.h"
#include "Base/Function.h"
#include "Base/Value.h"
#include "Base/String.h"
#include <string>
using namespace std;

namespace Public {
namespace Base {

//DB������
class BASE_API SqlDB_Row
{
public:
	SqlDB_Row();
	~SqlDB_Row();
	int getColumnCount();
	void putFiled(int index, const Value& val);
	const Value getField(int index = 0) const;
	const Value firstFiled() const; //��λ�α�Ϊ0
	const Value nextFiled() const; //�α����
private:
	struct SqlDB_RowInternal;
	SqlDB_RowInternal* internal;
};

//DB���ݲ�ѯ����
class BASE_API SqlDB_Recordset
{
public:
	SqlDB_Recordset() {}
	~SqlDB_Recordset() {}
	virtual shared_ptr<SqlDB_Row> getRow() = 0;
};

struct BASE_API SQLBlobValue
{
	SQLBlobValue() {}
	SQLBlobValue(const Value& val) :value(val) {}

	Value		value;
};

//����BLOB���������ݸ�ʽ
#define BLOB_STR(v) SQLBlobValue(v)
#define BLOB_BIN(d,l) SQLBlobValue(Value(std::string((const char*)d,l)))

//SQL��乹�����
class BASE_API SqlDB_Sql
{
public:
	//select * from aa where a = ?;
	SqlDB_Sql();
	SqlDB_Sql(const std::string& sqltmp);
	SqlDB_Sql(const char* sqltmp);
	SqlDB_Sql(const SqlDB_Sql& sql);
	~SqlDB_Sql();

	//���գ����ֵ�˳������滻Ϊval��������ַ������Զ����''
	SqlDB_Sql& operator << (const Value& val);
	SqlDB_Sql& operator << (const SQLBlobValue& val);
	SqlDB_Sql& operator = (const SqlDB_Sql& sql);
	std::string str(CharSetEncoding charset) const;

	std::vector<std::string> getBolbValue() const;

private:
	struct SqlDB_SqlInternal;
	SqlDB_SqlInternal* internal;
};

class BASE_API SqlDB_Connection
{
public:
	SqlDB_Connection() {}
	virtual ~SqlDB_Connection() {}

	virtual bool connect() = 0;
	virtual bool disconnect() = 0;

	//�����Լ�⣬true������false�쳣
	virtual bool integrityCheck() = 0;

	virtual bool exec(const SqlDB_Sql& sql,uint32_t* affectedrow = NULL) = 0;
	virtual bool exec(const std::vector<SqlDB_Sql>& sql, uint32_t* affectedrow = NULL) = 0;
	virtual CharSetEncoding charset() const = 0;

	virtual shared_ptr<SqlDB_Recordset> query(const SqlDB_Sql& sql) = 0;
	virtual uint32_t count(const SqlDB_Sql& sql) = 0;

	//���ݱ��ݵ��ļ�
	virtual bool backup(const std::string& tofile) { return false; }
	//���ļ��лָ�
	virtual bool recovery(const std::string& fromfile) { return false; }
};


//SQLITE���ݿ���ز���
class BASE_API SQLiteConnection : public SqlDB_Connection
{
	struct SQLiteConnectionInternal;

public:
	SQLiteConnection(const std::string &dbname, bool create, CharSetEncoding charset = CharSetEncoding_UTF8);
	virtual ~SQLiteConnection();

	virtual bool connect();
	virtual bool disconnect();

	virtual bool integrityCheck();

	virtual bool exec(const SqlDB_Sql &sql, uint32_t *affectedrow = NULL);
	virtual bool exec(const std::vector<SqlDB_Sql> &sql, uint32_t *affectedrow = NULL);

	virtual CharSetEncoding charset() const;

	virtual shared_ptr<SqlDB_Recordset> query(const SqlDB_Sql &sql);
	virtual uint32_t count(const SqlDB_Sql &sql);

	//���ݱ��ݵ��ļ�
	virtual bool backup(const std::string &tofile);
	//���ļ��лָ�
	virtual bool recovery(const std::string &fromfile);

private:
	SQLiteConnectionInternal *internal;
};

} // namespace Base
} // namespace Public

