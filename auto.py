#!/usr/bin/env python

from __future__ import print_function
import clang.cindex

# TODO: detect range-based for loops

def get_diag_info(diag):
    return { 'severity' : diag.severity,
             'location' : diag.location,
             'spelling' : diag.spelling,
             'ranges' : diag.ranges,
             'fixits' : diag.fixits }

def find_auto(node):
    """ Find all auto-declared variables
    """
    if node.type.kind == clang.cindex.TypeKind.AUTO:
        print('Found auto declaration: [line={}, col={}]'.format(
              node.location.line, node.location.column))
    # recurse for children of this node
    for c in node.get_children():
        find_auto(c)

def find_rfor(node):
    """ Find all range-based for loops
    """
    pass

if __name__=='__main__':
    import argparse
    from pprint import pprint
    parser = argparse.ArgumentParser(
            description='vet C++11 file for auto and range-based for loop usages')
    parser.add_argument('--auto', help='reject auto declarations', action="store_true")
    parser.add_argument('--rfor', help='reject range-based for loops',
                        action="store_true")
    (opts, args) = parser.parse_known_args()

    index = clang.cindex.Index.create()
    # we allow c++11 by default
    tu = index.parse(None, ['-std=c++11'] + args)
    pprint(('diags', map(get_diag_info, tu.diagnostics)))
    if opts.auto: find_auto(tu.cursor)
    if opts.rfor: find_rfor(tu.cursor)
