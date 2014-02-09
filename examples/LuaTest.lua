--[[
svr = tsnet.createUdpServer()

svr:start("127.0.0.1", 28888, 1, function()
    local req, host, port = svr:recvfrom()
    print(string.format("%s:%d | req(%d):%s", host, port, string.len(req), req))
    
    local rsp = svr:udpAccess(req, "127.0.0.1", 28889)
    print(rsp)

    svr:sendto(rsp, host, port)
    print("rsp ok")
end)

svr:release()
]]

--[[

svr = tsnet.createServer()

function svr.createUdpService("127.0.0.1", 28888):svc

function oidb:svc()

end


svr.registerTcpService("127.0.0.1", 28888, 10000).svc = function()

end

svr.registerUdpService("127.0.0.1", 28889).svc = function()

end

svr.runServices(10)

]]

--[[

server
    runServices
    createUdpService
    createTcpService

    numberOfServices
    services[]

service
    onRequest
    sendto
    recvfrom
    udpAccess

    parentThread
    _sock
    _netEventDriver
    
]]



err, svr = tsnet.createServer()
err, testSvc = svr:createUdpService("127.0.0.1", 28888)

function testSvc:onRequest()
    local err, req, host, port = testSvc:recvfrom()
    print(string.format("%s:%d | req(%d):%s", host, port, string.len(req), req))

    local err, rsp = testSvc:udpAccess(req, "127.0.0.1", 28889)
    print(rsp)

    local err = testSvc:sendto(rsp, host, port)
    print("rsp ok")

end

svr:runServices(2)
