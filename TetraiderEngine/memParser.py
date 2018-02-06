filepath = 'memoryOutput.txt'
importantLines = [];

with open(filepath) as fp:  
    line = fp.readline()
    cnt = 1
    while line:
        if "MemoryManager::Alloc() FAILED - No Available Slot" in line or "Allocating Memory" in line or "Successfully Allocated Memory" in line or "Pointer Allocated:" in line or "Pointer Being Freed:" in line or "Attempting to Free Memory" in line or "Successfully Freed Memory" in line or "Memory Block not found!" in line:
            importantLines.append(line.strip())
        #end if
        ##print("Line {}: {}".format(cnt, line.strip()))
        line = fp.readline()
        cnt += 1
    #end while
#end with

memory = {}
memoryAllocated = {}
memoryFreed = {}
index = 0
while index < len(importantLines):
    #print(importantLines[index])
    if index + 1 < len(importantLines):
        if "Pointer Allocated:" in importantLines[index]:
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


for key,val in memory.items():
    print(key, "=>", val)
#end for

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
