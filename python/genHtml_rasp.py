#!/usr/bin/env python
#coding:utf-8

import os,sys
import shutil

imageFormat = ('jpg', 'png', 'gif', 'jpeg')
videoFormat = ('mp4', 'flv', 'mkv', 'swf','webm', 'ogg')
delFormat = ('url', 'txt', 'rar', 'html')

def getCurPath():
    path = sys.path[0]
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)

def removeDir(dirPath):
    if not os.path.isdir(dirPath):
        return
    files = os.listdir(dirPath)
    try:
        for file in files:
            filePath=os.path.join(dirPath, file)
            if os.path.isfile(filePath):
                os.remove(filePath)
            elif os.path.isdir(filePath):
                removeDir(filePath)
        os.rmdir(dirPath)
    except Exception,e:
        print e

def reCreatDir(path):
    # removeDir(path)
    shutil.rmtree(path)
    os.makedirs(path)

def getfilelist(filepath, parentPath, genDir, tabnum = 1):
    filelist = os.listdir(filepath)
    simplePath = os.path.split(filepath)[1]

    if tabnum == 2:
        parentPath = simplePath
    if tabnum > 2:
        parentPath = parentPath + '/' + simplePath

    htmlBodyStr = ''

    for num in range(len(filelist)):
        filename = filelist[num]
        if filename[0] != '.':
            if os.path.isdir(filepath + '/' + filename):
                htmlBodyStr += '\t' * tabnum + '<a href="%s.html" target="_blank">%s</a></br>\n' %(filename, filename)
                getfilelist(filepath + '/' + filename, parentPath, genDir, tabnum + 1)
            else:
                if filename[filename.rfind('.') + 1 : ] in imageFormat:
                    if tabnum == 1:
                        htmlBodyStr += '\t' * tabnum + '<img src="%s" width="%dpx" height="%dpx"></br>\n' %(filename, 300, 300)
                    else:
                        htmlBodyStr += '\t' * tabnum + '<img src="%s/%s" width="%dpx" height="%dpx"></br>\n' %(parentPath, filename, 300, 300)
                elif filename[filename.rfind('.') + 1 : ] in videoFormat:
                    if tabnum == 1:
                        htmlBodyStr += '\t' * tabnum + '<a href="%s" target="_blank">%s</a></br>\n' %(filename, filename)
                    else:
                        htmlBodyStr += '\t' * tabnum + '<a href="%s/%s" target="_blank">%s</a></br>\n' %(parentPath, filename, filename)
                elif filename[filename.rfind('.') + 1 : ] in delFormat and tabnum == 1:
                    delFile = filepath + '/' + filename
                    print "---delFile: ", delFile
                    os.remove(delFile)

    htmlName = genDir + '/'+ simplePath + '.html'
    print "htmlName: ", htmlName
    htmlFile = open(htmlName, 'w+')
    htmlHeadStr = '<!DOCTYPE html>\n<page pageEncoding="utf-8">\n<meta content="text/html;charsetset=utf-8">'
    htmlHeadStr += '\n<html>'
    htmlHeadStr += '\n<head></head>'
    htmlHeadStr += '\n<body>\n'
    htmlTailStr = '\n</body>\n</html>'
    htmlFile.writelines(htmlHeadStr)
    htmlFile.writelines(htmlBodyStr)
    htmlFile.writelines(htmlTailStr)
    htmlFile.close()

def traversalDir(filepath, genDir):
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
        getfilelist(usefulpath, '', genDir)
        print 'genorate html file OK'

if __name__ == '__main__':
    # tmpPath = '/Users/boshin/Pictures/testHtml/'
    tmpPath = '/home/boshin/boshin/testHtml'
    srcPath = tmpPath
    # genDir = tmpPath
    genDir = srcPath + '/source'
    # srcPath = '/home/pi/Videos/'
    # genDir = '/usr/local/nginx-stream/html'

    reCreatDir(genDir)
    traversalDir(srcPath, genDir)
