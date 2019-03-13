#include"ngx_logic.h"


ngx_logic::ngx_logic()
{
    m_apkHander = sizeof(COMM_PKG_HEADER);
    m_msgHander = sizeof(STRUC_MSG_HEADER);
}
ngx_logic::~ngx_logic()
{

}