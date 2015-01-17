#pragma once

#include <opznet\shared_structs.h>
#include <string>

#define TO_UINT16 static_cast<unsigned short>

extern const size_t RESULT_BUNDLE_SIZE;
extern const size_t MAX_HP;
extern const size_t RANDOM_PENALTY_NUMBER;
extern const size_t MAX_CONTINUOUS_DRAW;

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

struct result_t
{
	int hand;
	int penalty;
	std::wstring log;

	result_t();
	result_t(const result_t & other);

	void clear(){hand = 0; penalty = 0; log.clear(); }
};
opznet::Packet & operator << (opznet::Packet & pstream, result_t & self);
opznet::Packet & operator >> (opznet::Packet & pstream, result_t & self);

struct basic_packet_t
{
	std::vector<std::pair<result_t,result_t>> results;

	void clear(){results.clear();}
};
opznet::Packet & operator << (opznet::Packet & pstream, basic_packet_t & self);
opznet::Packet & operator >> (opznet::Packet & pstream, basic_packet_t & self);