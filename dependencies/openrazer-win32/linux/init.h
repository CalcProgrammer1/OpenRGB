#ifndef INIT_H_
#define INIT_H_

#define KERN_WARNING
#define KERN_ALERT
#define KERN_CRIT

#define printk printf

inline unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base) {
	return strtoul(cp, endp, base);
}

inline void usleep(__int64 usec) {
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

inline void msleep(__int64 msec) {
	usleep(1000 * msec);
}

inline void usleep_range(__int64 usec1, __int64 usec2) {
	usleep((usec1 + usec2) / 2);
}

inline unsigned short eflip(unsigned short val) {
	return (val & 0xff) * 0xFF + (val >> 8);
}

#endif /* INIT_H_ */
