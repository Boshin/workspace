#!/usr/bin/env python
#coding:utf-8

import os,sys

imageFormat = ('jpg', 'png', 'gif', 'jpeg')
videoFormat = ('mp4', 'flv')

htmlHeadStr = '<!DOCTYPE html>\n<page pageEncoding="utf-8">\n<meta content="text/html;charsetset=utf-8">'
htmlHeadStr += '\n<html>'
htmlHeadStr += '\n<head></head>'
htmlHeadStr += '\n<body>'

htmlTailStr = '\n</body>\n</html>'

def getAllFiles(directory):
    files=[]
    for dirpath, dirnames,filenames in os.walk(directory):
        if filenames!=[]:
            for file in filenames:
                files.append(dirpath+'\\'+file)
    files.sort(key = len)
    return files

def showImageInHTML(imageTypes,savedir):
    files=getAllFiles(savedir)
    # files=getAllFiles(savedir+'\\pic')
    images=[f for f in files if f[f.rfind('.') + 1:] in imageTypes]
    tmpPic = images[1].decode('utf-8')
    print "11 images: ", tmpPic
    # os.path.getsize(tmpPic)
    # for ii in images
        # print "ii: ", ii "\n"
    # images=[item for item in images if os.path.getsize(item.decode('utf-8'))>5*1024]
    # images=['pic'+item[item.rfind('\\') + 1:] for item in images]
    images=[item[item.rfind('\\') + 1:] for item in images]
    print "22 images: ", images[1].decode('utf-8')
    newfile='%s/%s'%(savedir, 'images.html')
    with open(newfile,'w') as f:
        f.write('<!DOCTYPE html>\n<page pageEncoding="utf-8">\n<meta content="text/html;charsetset=utf-8">')
        # f.write('\n<page pageEncoding="utf-8">')
        # f.write('\n<meta content="text/html;charsetset=utf-8">')
        f.write('\n<html>')
        f.write('\n<head></head>')
        f.write('\n<body>')
        f.write('\n\t<div>\n')
        for image in images:
            f.write("\t\t<p>'%s'</p>\n" %image)
            f.write("\t\t<img src='%s' with='%dpx' height='%dpx'>\n" %(image, 300, 300))
        f.write('\t</div>')

        f.write('\n</body>\n</html>')
        # f.write('\n</html>')

    print 'success,images are wrapped up in %s'%newfile

#获取脚本文件的当前路径
def cur_file_dir():
    #获取脚本路径
    path = sys.path[0]
    #判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，如果是py2exe编译后的文件，则返回的是编译后的文件路径
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)

def getfilelist(filepath, tabnum=1):
    simplepath = os.path.split(filepath)[1]
    returnstr = simplepath + "目录<>" + "\n"
    returndirstr = " "
    returnfilestr = " "
    filelist = os.listdir(filepath)
    for num in range(len(filelist)):
        filename = filelist[num]
        if filename[0] != '.':
            # print "filename: ", filename, "\n"
            if os.path.isdir(filepath + "/" + filename):
                returndirstr += "\t" * tabnum + getfilelist(filepath + "/" + filename, tabnum + 1)
            else:
                returnfilestr += "\t" * tabnum + filename + "\n"
                if filename[filename.rfind('.') + 1 : ] in imageFormat:
                    print "====is image", filename, "\n"
                else:
                    print "+++ is video", filename, '\n'
    returnstr += returnfilestr + returndirstr
    return returnstr+ "\t" * tabnum + "</>\n"

def traversalDir(filepath):
    # path = raw_input("请输入文件路径:")
    path = filepath
    usefulpath = path.replace('\\', '/')
    if usefulpath.endswith("/"):
        usefulpath = usefulpath[ : -1]
    if not os.path.exists(usefulpath):
        print "路径错误!"
    elif not os.path.isdir(usefulpath):
        print "输入的不是目录!"
    else:
        getfilelist(usefulpath)
        filelist = os.listdir(usefulpath)
        o = open("test.xml","w+")
        o.writelines(getfilelist(usefulpath))
        o.close()
        print "成功！请查看test.xml文件"

if __name__ == '__main__':
    savedir=cur_file_dir()#获取当前.py脚本文件的文件路径
    traversalDir(savedir)
    showImageInHTML(('jpg','png','gif', 'jpeg'), savedir)#浏览所有jpg,png,gif文件
