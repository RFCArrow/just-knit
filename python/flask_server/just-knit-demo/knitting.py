
class Stitch:
    def __init__(self, _instruction, _progress):
        self.stitchType = _instruction[0]
        self.instruction = _instruction
        self.progress = _progress+1
        self.total = int(_instruction[1])

class Pattern:
    def __init__(self, instructionString):
        self.rowNumber=0
        self.stitchNumber=0
        self.instructionNumber=0
        self.isFinished = False
        self.pattern=[]
        self.instructionArray=[]
        self._parseInstructions(instructionString)
        self._createStitches()
        self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]

    def _createStitches(self):
        rowCounter = 0
        for row in self.instructionArray:
            stitchRow = []
            for instruction in row:
                stitches = []
                total = int(instruction[1])
                for i in range(total):
                    stitches.append(Stitch(instruction, i))
                stitchRow += stitches
            self.pattern.append(stitchRow)

    def _parseInstructions(self, instructionString):
        rows = instructionString.split('R:')
        # Delete first row because it is empty from the inital 'R:'
        del rows[0]
        for row in rows:
            instructions = row.split(',')
            instructionRow = []
            for instruction in instructions:
                instructionRow.append(instruction)
            self.instructionArray.append(instructionRow)


    def nextStitch(self):
        self.stitchNumber += 1
        previousStitch = self.currentStitch
        if previousStitch.progress==previousStitch.total:
            self.instructionNumber += 1
        if self.stitchNumber >= len(self.pattern[self.rowNumber]):
            self.stitchNumber = 0
            self.instructionNumber = 0
            self.rowNumber += 1
            if self.rowNumber >= len(self.pattern):
                self.isFinished=True
                return
        self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]

    def previousStitch(self):
        self.stitchNumber -= 1
        previousStitch = self.currentStitch
        if previousStitch.progress==1:
            self.instructionNumber -= 1
        if self.stitchNumber < 0:
            if self.rowNumber > 0:
                self.rowNumber -= 1
                self.stitchNumber = len(self.pattern[self.rowNumber])-1
                self.instructionNumber = len(self.instructionArray[self.rowNumber])-1
            else:
                # throw error
                self.stitchNumber = 0
                self.instructionNumber = 0
                return
        print("Reversed to: ", self.instructionNumber)
        self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]



    def getInstructionBuffer(self):
        row = self.instructionArray[self.rowNumber]
        instructionsLeft = len(row)-self.instructionNumber-1
        outbuffer = {}
        if(instructionsLeft<3):
            outbuffer['stitch1'] = '-'
        else:
            outbuffer['stitch1'] = row[self.instructionNumber+3]
        if(instructionsLeft<2):
            outbuffer['stitch2'] = '-'
        else:
            outbuffer['stitch2'] = row[self.instructionNumber+2]
        if(instructionsLeft<1):
            outbuffer['stitch3'] = '-'
        else:
            outbuffer['stitch3'] = row[self.instructionNumber+1]

        outbuffer['stitch4'] = row[self.instructionNumber]

        if(self.instructionNumber<1):
            outbuffer['stitch5'] = '-'
        else:
            outbuffer['stitch5'] = row[self.instructionNumber-1]
        if(self.instructionNumber<2):
            outbuffer['stitch6'] = '-'
        else:
            outbuffer['stitch6'] = row[self.instructionNumber-2]
        if(self.instructionNumber<3):
            outbuffer['stitch7'] = '-'
        else:
            outbuffer['stitch7'] = row[self.instructionNumber-3]

        outbuffer['total']      = self.currentStitch.total
        outbuffer['progress']   = self.currentStitch.progress
        outbuffer['rowNumber']        = self.rowNumber+1
        outbuffer['complete']   = self.stitchNumber
        outbuffer['remain']     = len(self.pattern[self.rowNumber])-self.stitchNumber

        return outbuffer

    def is_finished(self):
        return self.isFinished

    def setStitch(self, instructionMoves):

        if instructionMoves == 0:
                self.stitchNumber -= self.currentStitch.progress-1
                self.currentStitch=self.pattern[self.rowNumber][self.stitchNumber]
                return
        instructionNumber = self.instructionNumber+instructionMoves
        instructionNumber = instructionNumber % len(self.instructionArray[self.rowNumber])


        if instructionMoves > 0:
            while self.instructionNumber != instructionNumber:
                self.nextStitch()
            return

        if instructionMoves < 0:
            while self.instructionNumber != instructionNumber:
                self.previousStitch()
                if self.instructionNumber == 0 and self.rowNumber == 0:
                    break
            return




        

