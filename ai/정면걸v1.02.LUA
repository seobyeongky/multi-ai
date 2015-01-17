num = GetN()

if(num==0) then
	result = 1
else if(num==1)
	result = 2
else if(num==2)
	result = 0

else--start here

arr={}
jukarr={}
z=0

for i=0,num do
	z,x = GetRecordData(i)
	SaveData(i,x)
end

jukarr={}
for i=0,8 do
	jukarr{i}=0
end

for i=1,num do
	if(GetData(i)==0)
		if(GetData(i-1)==0)
			jukarr{0}=jukarr{0}+1
	else if(GetData(i)==0)
		if(GetData(i-1)==1)
			jukarr{1}=jukarr{1}+1
	else if(GetData(i)==0)
		if(GetData(i-1)==2)
			jukarr{2}=jukarr{2}+1
	else if(GetData(i)==1)
		if(GetData(i-1)==0)
			jukarr{3}=jukarr{3}+1
	else if(GetData(i)==1)
		if(GetData(i-1)==1)
			jukarr{4}=jukarr{4}+1
	else if(GetData(i)==1)
		if(GetData(i-1)==2)
			jukarr{5}=jukarr{5}+1
	else if(GetData(i)==2)
		if(GetData(i-1)==0)
			jukarr{6}=jukarr{6}+1
	else if(GetData(i)==2)
		if(GetData(i-1)==1)
			jukarr{7}=jukarr{7}+1
	else
		if(GetData(i-1)==2)
			jukarr{8}=jukarr{8}+1
	end
end

if(arr{num}==0)
	j=0
	for i=0,2 do
		if(jukarr{j}<jukarr{i})j=i
	end
else if(arr{num}==1)
	j=3
	for i=3,5 do
		if(jukarr{j}<jukarr{i})j=i
	end
else
	j=6
	for i=6,8 do
		if(jukarr{j}<jukarr{i})j=i
	end
end

Thistimejuk=0

if(j==0)
	Thistimejuk=0
else if(j==1)
	Thistimejuk=1
else if(j==2)
	Thistimejuk=2
else if(j==3)
	Thistimejuk=0
else if(j==4)
	Thistimejuk=1
else if(j==5)
	Thistimejuk=2
else if(j==6)
	Thistimejuk=0
else if(j==7)
	Thistimejuk=1
else
	Thistimejuk=2
end
--Thistimejuk

if(Thistimejuk==0)
	result = 2
else if(Thistimejuk==1)
	result = 0
else
	result = 1
end
-end here
end