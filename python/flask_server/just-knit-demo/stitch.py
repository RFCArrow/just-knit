
class Stitch:
    def __init__(self, _instruction, _progress):
        self.stitchType = _instruction[0]
        self.instruction = _instruction
        self.progess = _progress
        self.total = int(_instruction[1])

class Pattern:
    def __init__(self, instructionString):
        self.rowNumber=0
        self.stitchNumber=0
        self.instructionNumber=0
        self.isFinished = False
        # self.pattern
        # self.instructionArray
        self._parseInstructions()
        self._createStitches()
        self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]


    def _createStitches(self):
        rowCounter = 0
        for row in self.instructionArray:
            rowCounter += 1
            for instruction in row:
                total = int(instruction[1])
                for i in range(0,total-1)
                    self.pattern[rowCounter].append(Stitch(instruction, i))

    def _parseInstructions(self):
        rows = instuctionString.split('R:')
        rowCounter = 0 
        for row in rows:
            self.instructionArray[rowCounter].append(row.split(','))

    def nextStitch(self):
        previousStitch = self.currentStitch
        if previousStitch.progress==previousStitch.total:
            self.instructionNumber += 1
        self.stitchNumber += 1
        if(self.stitchNumber > len(self.pattern[self.rowNumber]):
            self.stitchNumber = 0
            self.instructionNumber = 0
            self.rowNumber += 1
            if(self.rowNumber > len(self.pattern):
                self.isFinished=True
                return
        self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]


    def getInstructionBuffer(self):
        row = self.instructionArray[rowNumber]
        instructionsLeft = len(row)-self.instructionNumber
        if(instructionsLeft<3):
            outbuffer['stitch1'] = '-'
        else:
            outbuffer['stitch1'] = row.[self.instructionNumber+3]
        if(instructionsLeft<2):
            outbuffer['stitch2'] = '-'
        else:
            outbuffer['stitch2'] = row.[self.instructionNumber+2]
        if(instructionsLeft<1):
            outbuffer['stitch3'] = '-'
        else:
            outbuffer['stitch3'] = row.[self.instructionNumber+1]

        outbuffer['stitch4'] = row[self.instructionNumber]

        if(self.instructionNumber<1):
            outbuffer['stitch5'] = '-'
        else:
            outbuffer['stitch5'] = row.[self.instructionNumber-1]
        if(self.instructionNumber<2):
            outbuffer['stitch6'] = '-'
        else:
            outbuffer['stitch6'] = row.[self.instructionNumber-2]
        if(self.instructionNumber<3):
            outbuffer['stitch7'] = '-'
        else:
            outbuffer['stitch7'] = row.[self.instructionNumber-3]

        outbuffer['total'] = self.currentStitch.total
        outbuffer['progress'] = self.currentStitch.progress
    
        return outbuffer

