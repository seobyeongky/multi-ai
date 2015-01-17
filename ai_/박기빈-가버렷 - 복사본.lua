n = GetN()

Strategy1AdjustSave = 1

PreviousStrategyNoSave = 99
StrategyScoreSaveBase = 99

function SaveScore()
	previousScore = GetFightData(n-1)
	previousStrategy = GetData(PreviousStrategyNoSave)
	savedScore = GetData(StrategyScoreSaveBase + previousStrategy)
	SaveData(StrategyScoreSaveBase + previousStrategy, savedScore + previousScore)
end

function GetBestStrategy()
	maxScore = -999
	maxScoreStrategy = 0
	
	for c=1, 3 do
		strategyOverallScore = GetData(StrategyScoreSaveBase + c)
		
		if strategyOverallScore > maxScore then
			maxScore = strategyOverallScore
			maxScoreStrategy = c
		end
	end
	
	return maxScoreStrategy
end

function Strategy1()
	SaveScore()
	SaveData(PreviousStrategyNoSave, 1)

	maxWin = -2
	maxWinStageStarts = -1
	for c=0, n-4 do
		pastResult = GetFightData(c)
		pastResult = pastResult + GetFightData(c+1)
		pastResult = pastResult + GetFightData(c+2)
		
		if maxWin <= pastResult then
			maxWin = pastResult
			maxWinStageStarts = c
		end
	end
	
	mSuspect, ySuspect = GetRecordData(maxWinStageStarts + 3)
	
	previousStage = GetFightData(n - 1)
	adjust = GetData(Strategy1AdjustSave)
	if previousStage == -1 then
		adjust = adjust + 2
		SaveData(Strategy1AdjustSave, adjust)
	elseif previousStage == 0 then
		adjust = adjust + 1
		SaveData(Strategy1AdjustSave, adjust + 1)
	end
	result = (ySuspect + adjust) % 3
end

function Strategy2()
	SaveScore()
	SaveData(PreviousStrategyNoSave, 2)
	
	m1, y1 = GetRecordData(n-3)
	m2, y2 = GetRecordData(n-2)
	m3, y3 = GetRecordData(n-1)
	for c=0, n-4 do
		pm1, py1 = GetRecordData(c)
		pm2, py2 = GetRecordData(c+1)
		pm3, py3 = GetRecordData(c+2)
		pm4, py4 = GetRecordData(c+3)
		
		if m1 == pm1 and m2 == pm2 and m3 == pm3 and y1 == py1 and y2 == py2 and y3 == py3 then
			result = (py4 + 1) % 3
		end
	end
	Strategy1()
end

function Strategy3()
	SaveScore()
	SaveData(PreviousStrategyNoSave, 3)
	
	m1, y1 = GetRecordData(n-3)
	m2, y2 = GetRecordData(n-2)
	m3, y3 = GetRecordData(n-1)
	for c=0, n-4 do
		pm1, py1 = GetRecordData(c)
		pm2, py2 = GetRecordData(c+1)
		pm3, py3 = GetRecordData(c+2)
		pm4, py4 = GetRecordData(c+3)
		
		if m1 == pm1 and m2 == pm2 and m3 == pm3 and y1 == py1 and y2 == py2 and y3 == py3 then
			result = (py4 + 1) % 3
			return
		end
	end
	Strategy1()
end

if n <= 6 then
	result = GetRandom() % 3
--elseif n <= 6 then
	--m, y = GetRecordData(n - 1)
	--result = y
elseif n <= 36 then
	Strategy1()
elseif n <= 66 then
	Strategy2()
elseif n <= 96 then
	Strategy3()
else
	chosenStrategy = GetBestStrategy()
	if chosenStrategy == 1 then
		Strategy1()
	elseif chosenStrategy == 2 then
		Strategy2()
	elseif chosenStrategy == 3 then
		Strategy3()
	end
end
