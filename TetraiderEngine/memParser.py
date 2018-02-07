filepath = 'output.txt'
importantLines = [];

importantLineStrings = ["MemoryManager::Alloc() FAILED - No Available Slot",
                        "Allocating Memory",
                        "Successfully Allocated Memory",
                        "Pointer Allocated:",
                        "Pointer Being Freed:",
                        "Attempting to Free Memory",
                        "Successfully Freed Memory",
                        "Memory Block not found!",
                        "RETURNED POINTER:",
                        "POINTER TO BE DELETED:",
                        "Allocating array.",
                        "Deallocating array.",
                        "Duplicate address returned!",
                        "Getting new block.",
                        "Recycling old block.",
                        "Size requested:",
                        "Size found:",
                        "MEMORY BLOCK ::",
                        "FINISHED"];

importantLineStrings2 = ["Duplicate address returned!",
                         "Getting new block.",
                         "Recycling old block.",
                         "Size requested:",
                         "Size found:",
                         "Pointer Being Freed:",
                         "Pointer Allocated:"];

logStrings = [  "Allocating Memory",
                "Successfully Allocated Memory",
                "Attempting to Free Memory",
                "Successfully Freed Memory"];

def lineInList(line, listOfStrings):
    for iLine in listOfStrings:
        if iLine in line:
            return True;
        #end if
    #end for
    return False;
#end def

with open(filepath) as fp:  
    line = fp.readline()
    cnt = 1
    while line:
        if lineInList(line, importantLineStrings):
            importantLines.append(line.strip())
        #end if
        ##print("Line {}: {}".format(cnt, line.strip()))
        line = fp.readline()
        cnt += 1
    #end while
#end with

memory = {}
pointerCount = {}
memoryAllocated = {}
memoryFreed = {}

def addPointerCount(pointer):
    if pointerCount.get(pointer, None) != None:
        pointerCount[pointer] = pointerCount[pointer] + 1
    #end if
    else:
        pointerCount[pointer] = 1
    #end else
#end def

def removePointerCount(pointer):
    if pointerCount.get(pointer, None) != None:
        pointerCount[pointer] = pointerCount[pointer] - 1
    #end if
##    else:
##        print("Trying to remove pointer that was gone already." )
##    #end else
#end def

newLogFile = open("pythonParsed.txt", "w");
index = 0
while index < len(importantLines):
    if lineInList(importantLines[index], importantLineStrings2):
        print(importantLines[index]);
    #end if
    if "MEMORY BLOCK ::" in importantLines[index]:
        newLogFile.write(importantLines[index] + "\n");
    #end if
    if "FINISHED" in importantLines[index]:
        newLogFile.write("\n\n\n\n");
    #end if
    if lineInList(importantLines[index], logStrings):
        newLogFile.write(importantLines[index] + "\n");
    #end if
    #print(importantLines[index])
    if index + 1 < len(importantLines):
        if "RETURNED POINTER:" in importantLines[index]:
            print("POINTER: {}".format(importantLines[index][17:]))
            addPointerCount(importantLines[index][17:])
        #end if
        if "POINTER TO BE DELETED:" in importantLines[index]:
            print("REMOVED POINTER: {}".format(importantLines[index][23:]))
            removePointerCount(importantLines[index][23:])
        #end if
        
        if "Pointer Allocated:" in importantLines[index]:
            ##addPointerCount(importantLines[index][19:])
            if "Successfully Allocated Memory" in importantLines[index + 1]:
                memory[ importantLines[index][19:] ] = "Allocated"
                memoryAllocated[ importantLines[index][19:] ] = "Allocated"
            #end if
            if "MemoryManager::Alloc() FAILED - No Available Slot" in importantLines[index + 1]:
                memory[ importantLines[index][19:] ] = "Failed to allocate"
                memoryAllocated[ importantLines[index][19:] ] = "Failed to allocate"
            #end if
        #end if
        if "Pointer Being Freed:" in importantLines[index]:
            ##addPointerCount(importantLines[index][21:])
            if "Successfully Freed Memory" in importantLines[index + 1]:
                memory[ importantLines[index][21:] ] = "Free"
                memoryFreed[ importantLines[index][21:] ] = "Free"
            #end if
            if "Memory Block not found!" in importantLines[index + 1]:
                memory[ importantLines[index][21:] ] = "Free? - Tried to Free Again"
                memoryFreed[ importantLines[index][21:] ] = "Free? - Tried to Free Again"
            #end if
        #end if
    #end if
    index = index + 1
#end while

newLogFile.close();

##for key,val in memory.items():
##    print(key, "=>", val)
###end for

print("\n============\n");

##for key,val in pointerCount.items():
##    print(key, "=>", val)
###end for

##print(" ===================== ")
##
##for key,val in memoryAllocated.items():
##    print(key, "=>", val)
###end for
##    
##print(" ===================== ")
##
##for key,val in memoryFreed.items():
##    print(key, "=>", val)
###end for
