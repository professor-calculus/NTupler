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
		std::string mday;
		int mday_num = time->tm_mday;
		if (mday_num < 10) mday = Form("0%d", mday_num);
		std::string hour = Form("%d", time->tm_hour);
		std::string min = Form("%d", time->tm_min);
		std::string sec = Form("%d", time->tm_sec);
		std::string timeStamp = year + "_" + mon + "_" + mday + "_" + hour + "_" + min + "_" + sec;
	    return timeStamp;
	}

} // closes namespace 'TimeStamp'
#endif