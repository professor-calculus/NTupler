#ifndef TIMESTAMP_H
#define TIMESTAMP_H


namespace TimeStamp{

	std::string GetTimeStamp()
	{
		time_t now = time(0);
		// tm *time = gmtime(&now); // gm time
		tm *time = localtime(&now); // local time  
		
		std::string year = Form("%d", time->tm_year + 1900);
		
		std::string mon;
		int mon_num = time->tm_mon + 1;
		if (mon_num < 10) mon = Form("0%d", mon_num);
		else mon = Form("%d", mon_num);

		std::string mday;
		int mday_num = time->tm_mday;
		if (mday_num < 10) mday = Form("0%d", mday_num);
		else mday = Form("%d", mday_num);
		
		std::string hour;
		int hour_num = time->tm_hour;
		if (hour_num < 10) hour = Form("0%d", hour_num);
		else hour = Form("%d", hour_num);

		std::string min;
		int min_num = time->tm_min;
		if (min_num < 10) min = Form("0%d", min_num);
		else min = Form("%d", min_num);

		std::string sec;
		int sec_num = time->tm_sec;
		if (sec_num < 10) sec = Form("0%d", sec_num);
		else sec = Form("%d", sec_num);

		std::string timeStamp = year + "_" + mon + "_" + mday + "_" + hour + "_" + min + "_" + sec;
	    return timeStamp;
	}

} // closes namespace 'TimeStamp'
#endif