--[[
    luaide  模板位置位于 Template/FunTemplate/NewFileTemplate.lua 其中 Template 为配置路径 与luaide.luaTemplatesDir
    luaide.luaTemplatesDir 配置 https://www.showdoc.cc/web/#/luaide?page_id=713062580213505
    author:{author}
    time:2019-03-19 11:13:05
]]
local allrevFuncTable={

}
local logincmghander = function ( _MsgPack )
    -- body
    net.nxg_debug("logincmghander")
    net.nxg_debug("_MsgPack:getId()".._MsgPack:getId())
    local  msghader = _MsgPack:getMsgHader()
    --local logicpro=msghader.pConn.logicPorcMutex
    local lock = net.CLock(_MsgPack:getLogicMutex())
    local _sendmsg = net.MsgPack(_MsgPack:getMsgHader(),5,_MsgPack:getId(),1,1024)
    _sendmsg:writeString("123456")
    _sendmsg:writeString("123456")
    _sendmsg:writeString("5555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555599999999999999999999999999999999999999999999999999999999999999999999999999999999999999")
    _sendmsg:send()

    lock:makeUnlock()
end
luatocppfunrevbuffer=function (_MsgPack)
    -- body
    net.nxg_debug("luatocppfunrevbuffer")
    net.nxg_debug(tostring(_MsgPack:getMsgID()))
    if(allrevFuncTable[tonumber(_MsgPack:getMsgID())]) then
        allrevFuncTable[tonumber(_MsgPack:getMsgID())](_MsgPack)
    end


end

registerAllrevCallBackFunk = function ()
    -- body
    allrevFuncTable[5]=logincmghander
end