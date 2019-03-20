



require("luatocppglobal")


print("hello lua")

function __G__TRACKBACK__(msg)
    local message = msg
    local msg = debug.traceback(msg, 3)
    -- if msg.len()<128 then
    --     print(msg)
    -- end
    print(msg)
    return msg
end
local function main()
    -- body
    print("lua mian")
    net.nxg_debug("ngx_log ok")
    local conn = net._STRUC_MSG_HEADER()
    conn.iCurrsequence=10;
    net.nxg_debug(tostring(conn.iCurrsequence))
    registerAllrevCallBackFunk()
    
end
local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end