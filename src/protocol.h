#pragma once

#define TO_UINT16 static_cast<unsigned short>

extern const size_t RESULT_BUNDLE_SIZE;

extern const size_t MAX_HP;

extern const size_t RANDOM_PENALTY_NUMBER;

enum sv_state_t
{
	SV_WAIT,
	SV_PLAY
};

enum hand_t
{
	SCISSORS = 0,
	ROCK = 1,
	PAPER = 2
};

enum sv_to_cl_t
{
	SV_TO_CL_STATE,
	SV_TO_CL_CHAT,
	SV_TO_CL_INFO,
	SV_TO_CL_REGISTER,
	SV_TO_CL_REGISTER_ACCEPTED,
	SV_TO_CL_REGISTER_DENIED,
	SV_TO_CL_BASIC_INFO,
	SV_TO_CL_GAME_DRAW,
	SV_TO_CL_SCRIPT_ERROR,
	SV_TO_CL_PLAY_CONTEXT,
	SV_TO_CL_UNREGISTER
};

enum cl_to_sv_t
{
	CL_TO_SV_CHAT,
	CL_TO_SV_REQUEST_REGISTER,
	CL_TO_SV_SCRIPT,
	CL_TO_SV_GAME_OVER
};