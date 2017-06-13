#!/usr/bin/env python2

__author__ = "Mislav Novakovic <mislav.novakovic@sartura.hr>"
__copyright__ = "Copyright 2017, Deutsche Telekom AG"
__license__ = "Apache 2.0"

import libyang_python2 as ly
import sys

try:
	ctx = ly.Context("/etc/sysrepo2/yang")
except Exception as e:
	print(e)
	err = ly.Error()
	print("err: %d" % err.err())
	print("vecode: %d" % err.vecode())
	print("errmsg: "+err.errmsg())
	print("errpath:"+err.errpath())
	print("errapptag:"+err.errapptag())

try:
	ctx = ly.Context("/etc/sysrepo/yang")
except Exception as e:
	print(e)

print("searchdir: " + ctx.get_searchdir())

module = ctx.get_module("ietf-interfaces", None)
if module is not None:
	print(module.name())
else:
	module = ctx.load_module("ietf-interfaces", None)
	if module is not None:
		print(module.name())
