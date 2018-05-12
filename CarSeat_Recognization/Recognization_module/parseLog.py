


def parseLog():
    fileName = "log_test.log"

    fp = open(fileName, "r")

    if fp is None:
        return None
    result = dict()
    while True:
        line = fp.readline()
        if line is None:
            break
        if len(line) == 0:
            break
        equal = line.find("=")
        if equal == -1:
            continue
        key = line[:equal]
        value = []
        equal = line.find("(")
        if equal == -1:
            continue
        end = line.find(")")
        if end == -1:
            continue
        #value.append(line[equal + 1:end])
        tmpType = line[equal + 1 : end]
        tmpLine = line[end + 1:]
        equal = tmpLine.find("=")
        if equal == -1:
            continue
        costTime = float(tmpLine[equal + 1:])
        value.append(costTime)
        value.append(tmpType)
        if key in result:
            result[key][0] += costTime
            result[key].append(value[1])
        else:
            result[key] = value
    fp.close()
    return result





def readlabel():
    labelFile = "output_labels.txt"
    fp = open(labelFile, "r")
    if fp is None:
        return None
    label = []
    while True:
        line = fp.readline()
        if line is None:
            break
        if len(line) == 0:
            break

        if line[-1] == '\n':
            line = line[:-2]
        label.append(line)
    return label

def analysis(label, recDict):

    print(recDict)
    print(type(recDict))
    for key in recDict.keys():
        value = recDict[key]
        totalTime = value[0]
        tmpLength = len(value)
        perTime = totalTime / (tmpLength - 1)
        print("key = %s, totalTime = %f, perImageTime = %f" %(key, totalTime, perTime))
        tmpDict = dict()
        for i in value:
            if i in tmpDict:
                tmpDict[i] += 1
            else:
                tmpDict[i] = 1
        print(tmpDict)



if __name__ == "__main__":
    label = readlabel()
    print(label)
    recDict = parseLog()
    analysis(label, recDict)
