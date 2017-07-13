#!/usr/bin/env python3

__author__ = "Mislav Novakovic <mislav.novakovic@sartura.hr>"
__copyright__ = "Copyright 2017, Deutsche Telekom AG"
__license__ = "Apache 2.0"

import libyang_python3 as ly
import sys

try:
    ctx = ly.Context("/etc/sysrepo/yang")
except Exception as e:
    print(e)
    err = ly.Error()
    print("err: %d" % err.err())
    print("vecode: %d" % err.vecode())
    print("errmsg: "+err.errmsg())
    print("errpath:"+err.errpath())
    print("errapptag:"+err.errapptag())

module = ctx.get_module("turing-machine", None)
if module is not None:
    print(module.name())
else:
    module = ctx.load_module("turing-machine", None)

try:
    node = ctx.parse_path("/etc/sysrepo/data/turing-machine.startup", ly.LYD_XML, 1)
except Exception as e:
    print(e)

if node is None:
    print("parse_path did not return any nodes")
else:
    print("tree_dfs\n")
    data_list = node.tree_dfs()
    if data_list is not None:
        for elem in data_list:
            print("name: " + elem.schema().name() + " type: " + str(elem.schema().nodetype()))

    print("\nChild of " + node.schema().name() + " is " + node.child().schema().name() + " \n ")

    print("tree_for\n")
    data_list = node.child().child().tree_for()
    if data_list is not None:
        for elem in data_list:
            print("child of " + node.child().schema().name() + " is: " + elem.schema().name() + " type: " + str(elem.schema().nodetype()))

    print("\nschema tree_dfs\n")
    schema_list = node.schema().tree_dfs()
    if schema_list is not None:
        for elem in schema_list:
            print("schema name: " + elem.name() + " type: " + str(elem.nodetype()))


