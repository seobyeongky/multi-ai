n = GetN()


if n<=1 then


	result=0

else
	x,y=GetRecordData(n-1)	
	
	if GetFightData(n-1)==1 then
	result=x	
			
	
	elseif GetFightData(n-1)==-1  then
	result=y
	

	else
	result=(x+1)%3
	end
end