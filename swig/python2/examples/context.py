#!/usr/bin/env python

__author__ = "Mislav Novakovic <mislav.novakovic@sartura.hr>"
__copyright__ = "Copyright 2017, Deutsche Telekom AG"
__license__ = "Apache 2.0"

import libyang_python2 as ly
import sys

ctx = ly.Context("/etc/sysrepo/yang")
print("searchdir: " + ctx.get_searchdir())
