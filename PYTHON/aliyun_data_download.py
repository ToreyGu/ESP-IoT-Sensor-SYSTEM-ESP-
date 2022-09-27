#!/usr/bin/env python
# coding=utf-8

from aliyunsdkcore.client import AcsClient
from aliyunsdkcore.acs_exception.exceptions import ClientException
from aliyunsdkcore.acs_exception.exceptions import ServerException
from aliyunsdkcore.http import method_type
from aliyunsdkiot.request.v20180120.InvokeDataAPIServiceRequest import InvokeDataAPIServiceRequest

accessKey = "YOUR ACCESSKEY"
accessSecret = "YOUR ACCESS SECRET"

client = AcsClient(accessKey, accessSecret, 'cn-shanghai')

request = InvokeDataAPIServiceRequest()
request.set_accept_format('json')
request.set_method(method_type.POST)
request.set_ApiSrn("YOUR OWN APISRN")

response = client.do_action_with_exception(request)
print(response)
