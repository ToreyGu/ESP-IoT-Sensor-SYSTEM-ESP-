#!/usr/bin/env python
# coding=utf-8

from aliyunsdkcore.client import AcsClient
from aliyunsdkcore.acs_exception.exceptions import ClientException
from aliyunsdkcore.acs_exception.exceptions import ServerException
from aliyunsdkcore.http import method_type
from aliyunsdkiot.request.v20180120.InvokeDataAPIServiceRequest import InvokeDataAPIServiceRequest

accessKey = "LTAI5tCaispGVTHSyaBmZCFx"
accessSecret = "DdCMrgGkJQN9Ow5Zmz5JsmK8pSTFe0"

client = AcsClient(accessKey, accessSecret, 'cn-shanghai')

request = InvokeDataAPIServiceRequest()
request.set_accept_format('json')
request.set_method(method_type.POST)
#request.page_num = 100
request.pagesize = 10000
request.set_ApiSrn("acs:iot:*:1210091327862341:serveapi/LA_ALGO/57f0f3edb5b0401892d7461b4d7479e9")

response = client.do_action_with_exception(request)
print(response)
