#include "sfx_mgr.h"

using namespace audiere;

#define REGISTER(x, y, z, q)		{_fileNames[x] = y; \
								_extNames[x] = z; \
								_fileNumbers[x] = q;}

opzSFXMgr::opzSFXMgr(void)
{
	REGISTER(SFX_LEAVE_CHAT, "leave_chat", "mp3", 1);
	REGISTER(SFX_READY, "ready", "mp3", 1);
	REGISTER(SFX_WIN, "win", "wav", 1);
	REGISTER(SFX_ENTER, "enter", "wav", 1);

	_device = OpenDevice();
	
	strcpy_s(_fileFullName, "sound\\");
	_fileNamePtr = _fileFullName + strlen(_fileFullName);
}

opzSFXMgr::~opzSFXMgr(void)
{
}

void opzSFXMgr::Play(sfx_t sfx)
{
	if(!_device || sfx == SFX_NULL) return;
	switch(_fileNumbers[sfx])
	{
	case 1:
		sprintf_s(_fileNamePtr, FULL_NAME_BUFFER_LENGTH - 7, "%s.%s",
			_fileNames[sfx], _extNames[sfx]);
		break;

	default:
		assert(_fileNumbers[sfx] > 1);
		sprintf_s(_fileNamePtr, FULL_NAME_BUFFER_LENGTH - 7, "%s%d.%s",
			_fileNames[sfx], rand() % _fileNumbers[sfx], _extNames[sfx]);
		break;
	}
	OutputStreamPtr stream(
		OpenSound(_device, _fileFullName, false));
	assert(stream != NULL);
	if(stream == NULL)
		return;
	stream->play();
	_streams.push_back(stream);
}

void opzSFXMgr::Update()
{
	for(std::list<OutputStreamPtr>::iterator i = _streams.begin();
		i != _streams.end();)
	{
		if((*i)->isPlaying())
			++i;
		else
		{
			(*i)->stop();
			*i = NULL;
			i = _streams.erase(i);
		}
	}
}