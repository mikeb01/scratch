//#define mcastaddr "ff02::3%en0"
#define mcastaddr "ff02::3%en0"

#define E(expr) CHECK((rc = (expr)) == 0, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, strerror(rc)), abort()))

int get_addr(const char* hostname,
             const char* service,
             int family,
             int socktype,
             int flags,
             struct sockaddr_in6* addr);

int joinGroup(int sockfd, int loopBack, int mcastHop,
              struct sockaddr_in6* addr);

int isMulticast(struct sockaddr_in6* addr);
