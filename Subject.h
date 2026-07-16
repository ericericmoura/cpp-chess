#pragma once

#include <functional>
#include <algorithm>
#include <utility>
#include <unordered_map>

namespace chess
{

template <typename ...Args>
class Subject
{
public:
	unsigned int Subscribe(std::function<void(Args...)> observer)
	{
		observers_map_.emplace(++next_id_, std::move(observer));
		return next_id_;
	}

	void Remove(unsigned int id)
	{
		observers_map_.erase(id);
	}

	void Notify(Args... args)
	{
		auto observers_copy = observers_map_;
		for (const auto& [id, observer] : observers_copy)
		{
			observer(args...);
		}
	}

private:
	unsigned int next_id_ = 0;
	std::unordered_map<unsigned int, std::function<void(Args...)>> observers_map_{};
};

} // namespace chess
