#include "protocol.h"

#include <string>

const size_t RESULT_BUNDLE_SIZE = 20;

const size_t MAX_HP = 300;

const size_t RANDOM_PENALTY_NUMBER = 10U;

const size_t MAX_CONTINUOUS_DRAW = 100U;

result_t::result_t()
	: hand(0)
	, penalty(0)
	, log()
{
}

result_t::result_t(const result_t & other)
	: hand(other.hand)
	, penalty(other.penalty)
	, log(other.log)
{
}

opznet::Packet & operator << (opznet::Packet & pstream, result_t & self)
{
	pstream
		<< self.hand
		<< self.penalty
		<< self.log;

	return pstream;
}

opznet::Packet & operator >> (opznet::Packet & pstream, result_t & self)
{
	pstream
		>> self.hand
		>> self.penalty
		>> self.log;

	return pstream;
}

opznet::Packet & operator << (opznet::Packet & pstream, basic_packet_t & self)
{
	pstream << self.results.size();

	for (auto & res : self.results)
	{
		pstream << res.first;
		pstream << res.second;
	}

	return pstream;
}

opznet::Packet & operator >> (opznet::Packet & pstream, basic_packet_t & self)
{
	size_t size;
	pstream >> size;
	self.results.reserve(size);
	self.results.clear();

	for (size_t i = 0; i < size; i++)
	{
		result_t left, right;
		pstream >> left;
		pstream >> right;
		self.results.push_back(std::pair<result_t,result_t>(left, right));
	}

	return pstream;
}