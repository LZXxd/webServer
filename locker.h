#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

/*信号量的类*/
class sem_locker
{
private:
    sem_t m_sem;

public:
    //初始化信号量
    sem_locker()
    {
	if(sem_init(&m_sem, 0, 0) != 0)
	    printf("sem init error\n");
    }
    //销毁信号量
    ~sem_locker()
    {
	sem_destroy(&m_sem);
    }

    //等待信号量
    bool wait()
    {
	return sem_wait(&m_sem) == 0;
    }
    //添加信号量
    bool add()
    {
	return sem_post(&m_sem) == 0;
    }
};


/*互斥 locker*/
class mutex_locker
{
private:
    pthread_mutex_t m_mutex;

public:
    mutex_locker()
    {
    	if(pthread_mutex_init(&m_mutex, NULL) != 0)
	    printf("mutex init error!");
    }
    ~mutex_locker()
    {
	pthread_mutex_destroy(&m_mutex);
    }

    bool mutex_lock()  //lock mutex
    {
	return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool mutex_unlock()   //unlock
    {
	return pthread_mutex_unlock(&m_mutex) == 0;
    }
};

/*条件变量 locker*/
class cond_locker
{
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;

public:
    // 初始化 m_mutex and m_cond
    cond_locker()
    {
	if(pthread_mutex_init(&m_mutex, NULL) != 0)
	    printf("mutex init error");
	if(pthread_cond_init(&m_cond, NULL) != 0)
	{   //条件变量初始化是被，释放初始化成功的mutex
	    pthread_mutex_destroy(&m_mutex);
	    printf("cond init error");
	}
    }
    // destroy mutex and cond
    ~cond_locker()
    {
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
    }
    //等待条件变量
    bool wait()
    {
	int ans = 0;
	pthread_mutex_lock(&m_mutex);
	ans = pthread_cond_wait(&m_cond, &m_mutex);
	pthread_mutex_unlock(&m_mutex);
	return ans == 0;
    }
    //唤醒等待条件变量的线程
    bool signal()
    {
	return pthread_cond_signal(&m_cond) == 0;
    }

    //唤醒all等待条件变量的线程
    bool broadcast()
    {
            return pthread_cond_broadcast(&m_cond) == 0;
    }
};

#endif
