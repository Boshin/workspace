#!/usr/bin/env python
#coding:utf-8

import os,sys
import shutil

imageFormat = ('jpg', 'png', 'gif', 'jpeg')
videoFormat = ('mp4', 'm4v', 'avi', 'wmv', 'mov', 'flv', 'mkv', 'swf','webm', 'ogg')
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
    if os.path.exists(path) and os.path.isdir(path):
        shutil.rmtree(path)
    os.makedirs(path)

def getfilelist(filepath, parentPath, genDir, tabnum = 1):
    filelist = os.listdir(filepath)
    simplePath = os.path.split(filepath)[1]
    htmlBodyStr = ''
    imgFiles = []
    videoFiles = []

    if tabnum == 2:
        parentPath = simplePath
    if tabnum > 2:
        parentPath = parentPath + '/' + simplePath

    isGen = 0
    for num in range(len(filelist)):
        filename = filelist[num]
        if filename[0] != '.':
            if os.path.isdir(filepath + '/' + filename):
                # htmlBodyStr += '\t' * tabnum + '<a href="%s.html" target="_top">%s</a></br>\n' %(filename, filename)
                htmlBodyStr += '\n\t\t\t<div class="content_folder">\n\t\t\t\t<img src="img_html/dir01.png" align="top" width="33px" height="35px" />'
                htmlBodyStr += '\n\t\t\t<a href="%s.html" target="_top">%s</a>\n\t\t</div>' %(filename, filename)
                getfilelist(filepath + '/' + filename, parentPath, genDir, tabnum + 1)
                isGen = True
            else:
                if filename[filename.rfind('.') + 1 : ] in imageFormat:
                    imgFiles.append(filename);
                elif filename[filename.rfind('.') + 1 : ] in videoFormat:
                    videoFiles.append(filename);
                elif filename[filename.rfind('.') + 1 : ] in delFormat and tabnum == 1:
                    delFile = filepath + '/' + filename
                    print "---delFile: ", delFile
                    # os.remove(delFile)

    if 0 < len(videoFiles):
        isGen = True
        for filename in videoFiles:
            if tabnum == 1:
                # htmlBodyStr += '\t' * tabnum + '<a href="%s" target="_blank">%s</a></br>\n' %(filename, filename)
                htmlBodyStr += '\n\t\t\t<div class="content_folder">\n\t\t\t\t<img src="img_html/video01.png" align="top" width="33px" height="35px" />'
                htmlBodyStr += '\n\t\t\t<a href="%s" target="_top">%s</a>\n\t\t</div>' %(filename, filename)
            else:
                # htmlBodyStr += '\t' * tabnum + '<a href="%s/%s" target="_blank">%s</a></br>\n' %(parentPath, filename, filename)
                htmlBodyStr += '\n\t\t\t<div class="content_folder">\n\t\t\t\t<img src="img_html/video01.png" align="top" width="33px" height="35px" />'
                htmlBodyStr += '\n\t\t\t\t<a href="%s/%s" target="_top">%s</a>\n\t\t\t</div>' %(parentPath, filename, filename)

    if 0 < len(imgFiles):
        isGen = True
        for filename in imgFiles:
            if tabnum == 1:
                htmlBodyStr += '\n\t\t\t<div class="img_folder">\n\t\t\t\t<img src="%s" width="33px" height="35px" />' %(filename)
                htmlBodyStr += '\n\t\t\t<a href="%s" target="_top">%s</a>\n\t\t</div>' %(filename, filename)
            else:
                # htmlBodyStr += '\t' * tabnum + '<a href="%s/%s" target="_blank">%s</a></br>\n' %(parentPath, filename, filename)
                htmlBodyStr += '\n\t\t\t<div class="img_folder">\n\t\t\t\t<img src="%s/%s" width="38px" height="30px" />' %(parentPath, filename)
                htmlBodyStr += '\n\t\t\t\t<a href="%s/%s" target="_top">%s</a>\n\t\t\t</div>' %(parentPath, filename, filename)

    if not isGen:
        print filepath + " needn't creat html file"
        return

    htmlName = genDir + '/'+ simplePath + '.html'
    print "htmlName: ", htmlName
    htmlHeadStr = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">'
    htmlHeadStr += '\n\n<page pageEncoding="utf-8">'
    htmlHeadStr += '\n<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">'
    htmlHeadStr += '\n<head>\n<meta http-equiv="content-type" content="text/html; charset=utf-8" />'
    htmlHeadStr += '\n<link rel="stylesheet" type="text/css" href="css/style.css" media="screen" />'
    htmlHeadStr += '\n<title>private web size</title>\n</head>'
    htmlHeadStr += '\n\n<body>\n\t<div class="frame">'

    htmlBodyStr_common_begin = '\n\t\t<div class="part_sild">\n\t\t\t'
    htmlBodyStr_common_begin += '<a href="index.html" target="_top" style="margin-left: 20px">Home page</a>\n\t\t</div>'
    htmlBodyStr_common_begin += '\n\t\t<div class="part_source">'
    htmlBodyStr_common_end = '\n\t\t</div>'
    htmlTailStr = '\n\t</div>\n</body>\n</html>'

    htmlFile = open(htmlName, 'w+')
    htmlFile.writelines(htmlHeadStr)
    htmlFile.writelines(htmlBodyStr_common_begin)
    htmlFile.writelines(htmlBodyStr)
    htmlFile.writelines(htmlBodyStr_common_end)
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
    srcPath = '/media/pi/Boshin/Videos/'
    genDir = '/usr/local/nginx-stream/html/source'

    reCreatDir(genDir)
    traversalDir(srcPath, genDir)
