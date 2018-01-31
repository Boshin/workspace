#!/usr/bin/python
import re
import sys
import os

maxLineLen = 100
forceIndent = 0

def _insertedNewLineInNormalStr(oldLine):
    if len(oldLine) > maxLineLen:
        chlen = oldLine.find("(") + 1
        # print "chlen:", chlen

        if forceIndent and chlen % 4:
            v = chlen % 4
            if v < 3:
                chlen -= v
            else:
                chlen += 4 - v
        repStr = ',\n'
        repStr += ' ' * chlen
        newLine = oldLine.replace(', ', repStr)
        print "newLine: ", repr(newLine)
        return newLine

def _handleBlockNote(oldLine, indentLen):
    tmpStr = oldLine
    print "############## ", indentLen
    newLine = ""
    newLine += ' ' * indentLen

    while len(tmpStr) > maxLineLen:
        newLine += tmpStr[0 : maxLineLen]
        newLine += os.linesep
        newLine += ' ' * indentLen
        tmpStr = tmpStr[maxLineLen : len(tmpStr)]

    newLine += tmpStr
    print "newline: %s" %newLine
    return newLine

def _handleLineNote(oldLine, indentLen):
    tmpStr = oldLine
    print "############## ", indentLen
    newLine = ""
    newLine += ' ' * indentLen
    splitLen = maxLineLen - 3 - indentLen

    while len(tmpStr) > maxLineLen:
        index = tmpStr.rfind(' ', splitLen - 20, splitLen)
        if -1 != index:
            print "rfind ok"
            newLine += tmpStr[0 : index]
            newLine += os.linesep
            newLine += ' ' * indentLen
            newLine += "//"
        else:
            print "rfind failed"
            newLine += tmpStr[0 : splitLen]
            newLine += os.linesep
            newLine += ' ' * indentLen
            newLine += "// "
            index = splitLen
        tmpStr = tmpStr[index : len(tmpStr)]

    newLine += tmpStr
    newLine += os.linesep
    print "newline: %s" %newLine
    return newLine

def _insertedNewLineInNoteStr(oldLine, indentLen):
    if -1 != oldLine.find('//', 0, 2):
        return _handleLineNote(oldLine, indentLen)
    else:
        return _handleBlockNote(oldLine, indentLen)

def getspacelength(perNoteStr):
    pattern =  "^(\s+)(.*)"
    m = re.match(pattern,perNoteStr)
    if m:
        return len(m.group(1))
    else:
        return 0;

def main():
    if len(sys.argv) != 3 and len(sys.argv) != 4:
        print "usage: %s in_file out_file" %sys.argv[0]
        exit()

    filein_name = sys.argv[1]
    fileout_name = sys.argv[2]
    if len(sys.argv) == 4 and sys.argv[3] == "forceIndent":
        forceIndent = 1

    if os.path.exists(filein_name):
        print "####%s#####" %filein_name
        pass
    else:
        print "%s not exist" %filein_name
        exit()

    f_in = open(filein_name, "r")

    noteReg = '//[^\r\n]*|/\*.*\*/'
    writeStr = ""
    for (lineNum, line) in enumerate(f_in):
        uStr = line.decode('utf-8')
        line = uStr
        print "---uStr: ", repr(uStr), "uStr len: ", len(uStr), "line: ", repr(line), "oStr len: ", len(line)
        if len(line) > maxLineLen:
            allNoteStr = re.findall(noteReg, line, re.S)
            if allNoteStr:
                for perNoteStr in allNoteStr:
                    print "line: %d" %lineNum, "perNoteStr: %s" %perNoteStr
                    spacePerLine = getspacelength(line)
                    print "space len: ", spacePerLine
                    if len(perNoteStr) > maxLineLen:
                        print "note len: %d" %len(perNoteStr)
                        writeStr += _insertedNewLineInNoteStr(perNoteStr, spacePerLine)
                        #writeStr += os.linesep
                    else:
                        print(getspacelength(line))
                        writeStr += ' ' * getspacelength(line)
                        writeStr += perNoteStr + os.linesep
                    tmpStr = line.replace(perNoteStr, '').rstrip() + os.linesep
                    if len(tmpStr) > maxLineLen:
                        writeStr += _insertedNewLineInNormalStr(tmpStr)
                    else:
                        writeStr += tmpStr
            else:
                writeStr += _insertedNewLineInNormalStr(line)
        else:
            writeStr += line

    f_in.close()
    f_out = open(fileout_name, "w")
    f_out.writelines(writeStr.encode('utf-8'))
    f_out.close()

if __name__ == "__main__":
    main()
