#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <assert.h>

class c_Stopwatch
{
private:
	const std::chrono::steady_clock::duration _zeroDuration = std::chrono::steady_clock::duration::zero();

	std::chrono::steady_clock::time_point _start;
	std::chrono::steady_clock::time_point _stop;
	std::chrono::steady_clock::duration _totalTime;
	bool _isPaused;

public:
	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;
	enum TimeFormat { _dd, _hh, _mm, _ss, _ms, _us, _ns }; // largest time unit for formatting: days, hours, minutes, seconds, milliseconds, microseconds, nanoseconds

	c_Stopwatch() :
		_isPaused(true),
		_totalTime(_zeroDuration)
	{}

	~c_Stopwatch() {}

	void Start()
	{
		assert(_isPaused);
		_isPaused = false;
		_start = std::chrono::steady_clock::now();
	}
	
	void Restart()
	{
		assert(_isPaused);
		_isPaused = false;
		_start = std::chrono::steady_clock::now();
	}

	void Stop()
	{
		assert(!_isPaused);
		_stop = std::chrono::steady_clock::now();
		_totalTime += _stop - _start;
		_isPaused = true;
	}

	void Reset()
	{
		_isPaused = true;
		_totalTime = _zeroDuration;
	}

	bool isPaused(void) const { return _isPaused; }

private:
	inline auto _totalChronosI() const
	{
		if (_isPaused)		return _totalTime;
		else				return _totalTime + (std::chrono::steady_clock::now() - _start); // running time including current run if any
	}

	template<typename T>						struct is_chrono_duration { static constexpr bool value = false; };
	template<typename Rep, typename Period>		struct is_chrono_duration<std::chrono::duration<Rep, Period>> { static constexpr bool value = true; };

	std::string FormattedDuration(std::chrono::steady_clock::duration t, const TimeFormat f = _hh, const std::streamsize setwsize = 2) const
	{
		std::ostringstream buffer;
		if (f == _dd)
		{
			days dd = std::chrono::duration_cast<days>(t);
			std::chrono::hours hh = std::chrono::duration_cast<std::chrono::hours>(t % days(1));
			std::chrono::minutes mm = std::chrono::duration_cast<std::chrono::minutes>(t % std::chrono::hours(1));
			std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(t % std::chrono::minutes(1));
			buffer << std::setfill('0') << std::setw(setwsize) << dd.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << hh.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << mm.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << ss.count();
		}
		else if (f == _hh)
		{
			std::chrono::hours hh = std::chrono::duration_cast<std::chrono::hours>(t);
			std::chrono::minutes mm = std::chrono::duration_cast<std::chrono::minutes>(t % std::chrono::hours(1));
			std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(t % std::chrono::minutes(1));
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t % std::chrono::seconds(1));
			buffer << std::setfill('0') << std::setw(setwsize) << hh.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << mm.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << ss.count() << ".";
			buffer << std::setfill('0') << std::setw(3) << ms.count();
		}
		else if (f == _mm)
		{
			std::chrono::minutes mm = std::chrono::duration_cast<std::chrono::minutes>(t);
			std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(t % std::chrono::minutes(1));
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t % std::chrono::seconds(1));
			buffer << std::setfill('0') << std::setw(setwsize) << mm.count() << ":";
			buffer << std::setfill('0') << std::setw(2) << ss.count() << ".";
			buffer << std::setfill('0') << std::setw(3) << ms.count();
		}
		else if (f == _ss)
		{
			std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(t);
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t % std::chrono::seconds(1));
			std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(t % std::chrono::milliseconds(1));
			buffer << std::setfill('0') << std::setw(setwsize) << ss.count() << ".";
			buffer << std::setfill('0') << std::setw(3) << ms.count() << ",";
			buffer << std::setfill('0') << std::setw(3) << us.count();
		}
		else if (f == _ms)
		{
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t % std::chrono::seconds(1));
			std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(t % std::chrono::milliseconds(1));
			std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t % std::chrono::microseconds(1));
			buffer << std::setfill('0') << std::setw(setwsize) << ms.count() << ",";
			buffer << std::setfill('0') << std::setw(3) << us.count() << ",";
			buffer << std::setfill('0') << std::setw(3) << ns.count();
		}
		else if (f == _us)
		{
			std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(t);
			std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t % std::chrono::microseconds(1));
			buffer << std::setfill('0') << std::setw(setwsize) << us.count() << ",";
			buffer << std::setfill('0') << std::setw(3) << ns.count();
		}
		else if (f == _ns)
		{
			std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
			buffer << std::setfill('0') << std::setw(setwsize) << ns.count() << ",";
		}
		else
			throw;

		return buffer.str();
	}

public:
	template<typename Dur = std::chrono::duration<double, std::chrono::seconds::period>>
	inline typename Dur::rep TotalChrono() const
	{
		static_assert(is_chrono_duration<Dur>::value, "template Duration must be a std::chrono::duration");
		return std::chrono::duration_cast<Dur>(_totalChronosI()).count();
	}
	inline double TotalDays() const { return TotalChrono<std::chrono::duration<double, days::period>>(); }
	inline double TotalHours() const { return TotalChrono<std::chrono::duration<double, std::chrono::hours::period>>(); }
	inline double TotalMinutes() const { return TotalChrono<std::chrono::duration<double, std::chrono::minutes::period>>(); }
	inline double TotalSeconds() const { return TotalChrono<std::chrono::duration<double, std::chrono::seconds::period>>(); }
	inline double TotalMilliseconds() const { return TotalChrono<std::chrono::duration<double, std::chrono::milliseconds::period>>(); }
	inline double TotalMicroseconds() const { return TotalChrono<std::chrono::duration<double, std::chrono::microseconds::period>>(); }
	inline double TotalNanoseconds() const { return TotalChrono<std::chrono::duration<double, std::chrono::nanoseconds::period>>(); }

	std::string FormattedDurationLegend(const TimeFormat f = _hh) const
	{
		if (f == _dd)			return "dd:hh:mm:ss";
		else if (f == _hh)		return "hh:mm:ss.ms";
		else if (f == _mm)		return "mm,ss.ms";
		else if (f == _ss)		return "ss.ms,us";
		else if (f == _ms)		return "ms,us,ns";
		else if (f == _us)		return "us,ns";
		else if (f == _ns)		return "ns";
		else					throw;
	}

	std::string FormattedTotalChrono(const TimeFormat format = _hh, const std::streamsize st = 2) const { return FormattedDuration(_totalChronosI(), format, st); }
};

#endif // STOPWATCH_H