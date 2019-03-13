
#ifndef __DATAHANDLE_MYSQL_H__
#define __DATAHANDLE_MYSQL_H__

#include"mysql/mysql.h"

class data_handle_mysql{
public:
    //static data_handle_mysql* instance();
    bool init(char* ip,int port,
                        char* db_name,char* uname,char* pwd);
    bool connect(void(*open_cb)(const char* err,void *context,void* udata),void *udata=nullptr);
    
    bool close(void *context);
    bool query(void* context,
                    char* sql,
                    void(*query_cb)(const char* err,MYSQL_RES* result,void*udata),void* udata=nullptr);
private:
    data_handle_mysql();
private:
    char* m_ip=nullptr;
    int   m_port=3306;
    char* m_dbname=nullptr;
    char* m_username=nullptr;
    char* m_password=nullptr;

    MYSQL m_globleCont;

    static data_handle_mysql* m_datainstance;

public:
    static data_handle_mysql* GetInstance() 
	{	
		if(m_datainstance == nullptr)
		{
			//锁
			if(m_datainstance == nullptr)
			{					
				m_datainstance = new data_handle_mysql();
				static CGarhuishou cl; 
			}
			//放锁		
		}
		return m_datainstance;
	}	
	class CGarhuishou  //类中套类，用于释放对象
	{
	public:				
		~CGarhuishou()
		{
			if (data_handle_mysql::m_datainstance)
			{						
				delete data_handle_mysql::m_datainstance;				
				data_handle_mysql::m_datainstance = nullptr;				
			}
		}
	};


};

#endif // MACRO
