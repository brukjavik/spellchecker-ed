import time

class HashTable:

    def __init__(self, fileNameDictionary):
        self.fileDictionary = fileNameDictionary
        self.Dictonary = self.LoadDictionary(self.fileDictionary)

    def LoadDictionary(self, fileNameDictionary):
        self.fileDictionary = fileNameDictionary
        file = open(self.fileDictionary, "r")
        dictionary = {}
        line = file.readline()

        while line:
            line = line.rstrip()
            if line not in dictionary:
                dictionary[line] = line
            line = file.readline()

        file.close()
        return dictionary


    def Searching(self, fileToCompareWithDictionary):

        counterHowManyWords = 0
        counterNotFoundWords = 1 # Seta o índice em 1
        text = open(fileToCompareWithDictionary, "r")
        finalReport = open("results.txt", "w")

        fileLine = text.readline()
        start = time.time()

        while fileLine:
            line = fileLine.split()
            for string in line:
                counterHowManyWords += 1
                if string not in self.Dictonary:
                    finalReport.write('%d' % counterNotFoundWords + "- " + string+"\n")
                    counterNotFoundWords += 1
            fileLine = text.readline()
        text.close()

        final = time.time()

        totalTime = (final - start) * 1000

        counterNotFoundWords -= 1 # Seta o valor em 0
        finalReport.write("Número total de palavras do texto: " + '%d' % counterHowManyWords +"\n")
        finalReport.write("Número de palavras não encontradas no texto: " + '%d' % counterNotFoundWords+"\n")
        finalReport.write("Tempo total da verificação: " + '%d' % totalTime+" ms" +"\n")

        finalReport.close()

def main():

    dictionary = "dicionario.txt"
    text = "text.txt"

    hashTable = HashTable(dictionary)
    hashTable.Searching(text)

main()