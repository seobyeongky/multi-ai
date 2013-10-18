S = 0
R = 1
P = 2

VS = 0
VR = 0
VP = 0

lose = -1
win = 1
draw = 0

strategyNumber = 1

N = GetN()
PMine, PEnemy = GetRecordData(N - 1)
PResult = GetFightData(N - 1)

result = 4

winLate = 0

function winFunction(input)
	if input == S then
		return R
	elseif input == R then
		return P
	elseif inpuy == P then
		return S
	end
end

function AVS(a, b)
	if a == b then
		return draw
	else
		if a == S then
			if b == R then
				return lose
			else
				return win
			end
		elseif a == R then
			if b == P then
				return lose
			else
				return win
			end
		elseif a == P then
			if b == S then
				return lose
			else
				return win
			end
		end
	
	end
end


function maxABC(a, b, c)
	if a >= b and a >= c then
		return a
	elseif b >= c and b >= a then
		return b
	elseif c >= a and c >= b then
		return c
	end
end

function makeClikeA(a, b, c)
	if a == b then
		return c
	else
		if a == S then
			if b == R then
				if c == R then
					return S
				elseif c == P then
					return R
				else 
					return P
				end
			else
				if c == R then
					return P
				elseif c == P then
					return S
				else 
					return R
				end
			end
		elseif a == R then
			if b == P then
				if c == R then
					return S
				elseif c == P then
					return R
				else 
					return P
				end
			else
				if c == R then
					return P
				elseif c == P then
					return S
				else 
					return R
				end
			end
		elseif a == P then
			if b == S then
				if c == R then
					return S
				elseif c == P then
					return R
				else 
					return P
				end
			else
				if c == R then
					return P
				elseif c == P then
					return S
				else 
					return R
				end
			end
		end
	
	end
	

end


if strategyNumber == 1 then
	
	if N > 2 then
		
		for i = 1, N - 1 do
			
			forResult = GetFightData(i)
			
			forMine, forEnemy  = GetRecordData(i)
			
			forPResult = GetFightData(i - 1)
			forPMine, forPEnemy  = GetRecordData(i - 1)
			
			winLate = winLate + forResult
			--winLate°è»ê
			
			forEnemy = makeClikeA(forMine,PMine,forEnemy)
			forMine = PMine
			forPEnemy = makeClikeA(forPMine,PMine,forPEnemy)
			forPMine = PMine
			
			if forPResult == PResult then
			
				if forResult == win then
				
					if forPMine == R then
						VR = VR + 1
					elseif forPMine == S then
						VS = VS + 1
					else 
						VP = VP + 1
					end
					
				elseif forResult == lose then
					if forPMine == R then
						VS = VS + 1
					elseif forPMine == S then
						VP = VP + 1
					else 
						VR = VR + 1
					end
					
				else
					if forPMine == R then
						VP = VP + 1
					elseif forPMine == S then
						VR = VR + 1
					else 
						VS = VS + 1
					end
					
				end
				
			end
			
			
		end
		
		if VS == maxABC(VS, VP, VR) then
			result = S
		elseif VR == maxABC(VS, VP, VR) then
			result = R
		else
			result = P
		end
		
		if N > 500 and winLate < 0 then
			strategyNumber = 2
		end
		
	else
		result = N
	end
	
end

winLate = 0
if strategyNumber == 2 then

	k, m = GetFightData(N-1)
	result = winFunction(k)
end









