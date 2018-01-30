#!/usr/bin/env python
#coding:utf-8

import os,sys

imageFormat = ('jpg', 'png', 'gif', 'jpeg')
videoFormat = ('mp4', 'flv')

# 获取脚本文件的当前路径
def cur_file_dir():
    # 获取脚本路径
    path = sys.path[0]
    # 判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，
    # 如果是py2exe编译后的文件，则返回的是编译后的文件路径
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)

def getfilelist(filepath, tabnum = 1):
    simplepath = os.path.split(filepath)[1]
    # returnstr = simplepath + '目录<>' + '\n'
    returnstr = simplepath + '\n'
    filelist = os.listdir(filepath)

    htmlHeadStr = '<!DOCTYPE html>\n<page pageEncoding="utf-8">\n<meta content="text/html;charsetset=utf-8">'
    htmlHeadStr += '\n<html>'
    htmlHeadStr += '\n<head></head>'
    htmlHeadStr += '\n<body>\n'
    htmlTailStr = '\n</body>\n</html>'
    htmlBodyStr = ''

    htmlName = filepath + '/'+ simplepath + '.html'
    htmlFile = open(htmlName, 'w+')
    htmlFile.writelines(htmlHeadStr)

    for num in range(len(filelist)):
        filename = filelist[num]
        if filename[0] != '.':
            if os.path.isdir(filepath + '/' + filename):
                htmlBodyStr += '\t' * tabnum + '<a href="%s/%s.html" target="_blank">%s</a></br>\n' %(filename, filename, filename)
                getfilelist(filepath + '/' + filename, tabnum + 1)
            else:
                if filename[filename.rfind('.') + 1 : ] in imageFormat:
                    htmlBodyStr += '\t' * tabnum + '<img src="%s" width="%dpx" height="%dpx"></br>\n' %(filename, 300, 300)
                elif filename[filename.rfind('.') + 1 : ] in videoFormat:
                    htmlBodyStr += '\t' * tabnum + '<a href="%s" target="_blank">%s</a></br>\n' %(filename, filename)
    print "htmlBodyStr \n",  htmlBodyStr
    htmlFile.writelines(htmlBodyStr)
    htmlFile.writelines(htmlTailStr)
    htmlFile.close()

def traversalDir(filepath):
    # path = raw_input('请输入文件路径:')
    path = filepath
    usefulpath = path.replace('\\', '/')
    if usefulpath.endswith('/'):
        usefulpath = usefulpath[ : -1]
    if not os.path.exists(usefulpath):
        print 'path error'
    elif not os.path.isdir(usefulpath):
        print 'is not a directory'
    else:
        getfilelist(usefulpath)
        print 'genorate html file OK'

if __name__ == '__main__':
    # savedir = cur_file_dir()#获取当前.py脚本文件的文件路径
    savedir = '/home/boshin/boshin/testHtml'
    traversalDir(savedir)
