#!/usr/bin/env python

from __future__ import print_function
import clang.cindex

GLOBAL_INCLUDE_DIRECTORY='/usr/lib/clang/3.9/include/'

def insert_into_function_list(method):
    """ Insert a clang C method into the method list which gets lazy loaded
    """
    # TODO: does this need to be placed into the functionList in alphabetical order?
    clang.cindex.functionList.append(method)

# We load the cindex isInSystemHeader method which is not provided by default by the
# default clang bindings.
insert_into_function_list(
        ("clang_Location_isInSystemHeader", [clang.cindex.SourceLocation]))

def get_diag_info(diag):
    return "{}:{}:{} {} ".format(diag.location.file.name,
           diag.location.line, diag.location.column, diag.spelling)

def visitor(node, auto, rfor):
    """ Find all auto-declared variables and range-based for loops
    """
    try:
        # ignore system headers...
        if clang.cindex.conf.lib.clang_Location_isInSystemHeader(node.location) == 0:
            if auto == True and node.kind.is_declaration() and \
               node.type.kind == clang.cindex.TypeKind.AUTO:
                # detect auto
                print('Found auto declaration: {}:{}:{}'.format(
                      node.location.file.name, node.location.line, node.location.column))
            if rfor == True and node.kind.is_statement() and \
               node.kind == clang.cindex.CursorKind.CXX_FOR_RANGE_STMT:
                # detect ranged-based for loops
                print('Found range-based for loop: {}:{}:{}'.format(
                      node.location.file.name, node.location.line, node.location.column))
    except ValueError:
        # unfortunately, the python bindings are incomplete and may throw ValueError
        # when we encounter a CursorKind which has not been explicitly written into the
        # bindings. We silently ignore them because they are not relevant for our use
        # case.
        pass
    # recurse for children of this node
    for c in node.get_children():
        visitor(c, auto, rfor)

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
    # we allow c++11 by default, and we spoof the system include directory.
    default_args = ['-isystem', GLOBAL_INCLUDE_DIRECTORY, '-std=c++11']
    tu = index.parse(None, default_args + args)
    pprint(('diags', map(get_diag_info, tu.diagnostics)))
    if opts.auto or opts.rfor:
        visitor(tu.cursor, opts.auto, opts.rfor)
