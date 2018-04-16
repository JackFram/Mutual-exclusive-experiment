#include <unistd.h>

#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>


static const int N = 5;
static const int kItemRepositorySize  = 4; // 仓库最大存储量
static const int kItemsToProduce  = 10;   // 计划生产多少个商品

struct ItemRepository {
    int item_buffer[kItemRepositorySize];
    size_t read_position;//记录消费者消费位置
    size_t write_position;//记录生产者生产的位置
    size_t produced_item_counter;//记录生产数量
    size_t consumed_item_counter;//记录消费数量
    std::mutex mtx;//互斥锁，用来解决互斥问题
    std::mutex produced_item_counter_mtx;
    std::mutex consumed_item_counter_mtx;
    std::condition_variable repo_not_full;//用来记录仓库是否满了
    std::condition_variable repo_not_empty;//用来记录仓库是否是空的
} gItemRepository;

std::mutex output;

typedef struct ItemRepository ItemRepository;


void ProduceItem(ItemRepository *ir, int item)
{
    std::unique_lock<std::mutex> lock(ir->mtx);//生产的时候将互斥锁上锁
    while(((ir->write_position + 1) % kItemRepositorySize)
          == ir->read_position) { // 如果生产数量超过仓库存储量，等待
        std::cout << "Producer is waiting for an empty slot...\n";
        (ir->repo_not_full).wait(lock);//如果不满的话，先将该锁释放再等待notify_all
    }

    (ir->item_buffer)[ir->write_position] = item;//生产物品
    (ir->write_position)++;//将生产位置前移一个位置

    if (ir->write_position == kItemRepositorySize)
        ir->write_position = 0;

    (ir->repo_not_empty).notify_all();
    lock.unlock();//生产完后，释放互斥锁。
}

int ConsumeItem(ItemRepository *ir)
{
    int data;
    std::unique_lock<std::mutex> lock(ir->mtx);//在消费者取物品时，锁上互斥锁
    // 当仓库是空的时候进行等待
    while(ir->write_position == ir->read_position) {
        std::cout << "Consumer is waiting for items...\n";
        (ir->repo_not_empty).wait(lock);
    }

    data = (ir->item_buffer)[ir->read_position];
    (ir->read_position)++;//与生产时情况相同

    if (ir->read_position >= kItemRepositorySize)
        ir->read_position = 0;

    (ir->repo_not_full).notify_all();
    lock.unlock();

    return data;
}

void ProducerTask()
{
    bool ready_to_exit = false;
    while(1) {
        sleep(1);
        std::unique_lock<std::mutex> lock(gItemRepository.produced_item_counter_mtx);
        if (gItemRepository.produced_item_counter < kItemsToProduce) {
            ++(gItemRepository.produced_item_counter);
            ProduceItem(&gItemRepository, gItemRepository.produced_item_counter);
            std::unique_lock<std::mutex> lock1(output);
            std::cout << "Producer thread " << std::this_thread::get_id()
                      << " is producing the " << gItemRepository.produced_item_counter
                      << "^th item" << std::endl;
            lock1.unlock();
        } else ready_to_exit = true;
        lock.unlock();
        if (ready_to_exit == true) break;
    }
    std::unique_lock<std::mutex> lock1(output);
    std::cout << "Producer thread " << std::this_thread::get_id()
              << " is exiting..." << std::endl;
    lock1.unlock();
}

void ConsumerTask()
{
    bool ready_to_exit = false;
    while(1) {
        sleep(1);
        std::unique_lock<std::mutex> lock(gItemRepository.consumed_item_counter_mtx);
        if (gItemRepository.consumed_item_counter < kItemsToProduce) {
            int item = ConsumeItem(&gItemRepository);
            ++(gItemRepository.consumed_item_counter);
            std::unique_lock<std::mutex> lock1(output);
            std::cout << "Consumer thread " << std::this_thread::get_id()
                      << " is consuming the " << item << "^th item" << std::endl;
            lock1.unlock();
        } else ready_to_exit = true;
        lock.unlock();
        if (ready_to_exit == true) break;
    }
    std::unique_lock<std::mutex> lock1(output);
    std::cout << "Consumer thread " << std::this_thread::get_id()
              << " is exiting..." << std::endl;
    lock1.unlock();
}

void InitItemRepository(ItemRepository *ir)
{
    ir->write_position = 0;
    ir->read_position = 0;
    ir->produced_item_counter = 0;
    ir->consumed_item_counter = 0;
}

//philosopher problem

typedef struct Table{
    std::mutex chop[5];
}Table;

Table tb;

void philosopher(int i)
{
    if(i%2==0)
    {
        std::unique_lock<std::mutex> lock(tb.chop[i]);
        sleep(1);
        std::unique_lock<std::mutex> lock1(output);
        std::cout<<i<<" pickup left fork!"<<std::endl;
        lock1.unlock();
        lock.unlock();
        std::unique_lock<std::mutex> lock2(tb.chop[(i+1)%N]);
        sleep(1);
        std::unique_lock<std::mutex> lock3(output);
        std::cout<<i<<" pickup right fork and start to eat!"<<std::endl;
        lock3.unlock();
        lock2.unlock();
        sleep(3);
        std::unique_lock<std::mutex> lock4(output);
        std::cout<<i<<" is thinking!\n"<<std::endl;
        lock4.unlock();
    }
    else
    {
        std::unique_lock<std::mutex> lock(tb.chop[(i+1)%N]);
        sleep(1);
        std::unique_lock<std::mutex> lock1(output);
        std::cout<<i<<" pickup right fork!"<<std::endl;
        lock1.unlock();
        lock.unlock();
        std::unique_lock<std::mutex> lock2(tb.chop[i%N]);
        sleep(1);
        std::unique_lock<std::mutex> lock3(output);
        std::cout<<i<<" pickup left fork and start to eat!"<<std::endl;
        lock3.unlock();
        lock2.unlock();
        sleep(3);
        std::unique_lock<std::mutex> lock4(output);
        std::cout<<i<<" is thinking!\n"<<std::endl;
        lock4.unlock();
    }
}

void Consumer_Producer(){
    InitItemRepository(&gItemRepository);
    std::thread producer1(ProducerTask);
    std::thread producer2(ProducerTask);
    std::thread producer3(ProducerTask);
    std::thread producer4(ProducerTask);

    std::thread consumer1(ConsumerTask);
    std::thread consumer2(ConsumerTask);
    std::thread consumer3(ConsumerTask);
    std::thread consumer4(ConsumerTask);

    producer1.join();
    producer2.join();
    producer3.join();
    producer4.join();

    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();
}

void Phi(){
    std::thread phi1(philosopher,0);
    std::thread phi2(philosopher,1);
    std::thread phi3(philosopher,2);
    std::thread phi4(philosopher,3);
    std::thread phi5(philosopher,4);

    phi1.join();
    phi2.join();
    phi3.join();
    phi4.join();
    phi5.join();

}

//读者和写者问题
typedef struct File_system{
    std::mutex x,y,z,wsem,rsem;
    int readcount = 0;
    int writecount = 0;
};

File_system fs;

void reader(){
    std::unique_lock<std::mutex> lock4(fs.wsem);
    lock4.unlock();
    std::unique_lock<std::mutex> lock1(fs.z);
    std::unique_lock<std::mutex> lock2(fs.rsem);
    std::unique_lock<std::mutex> lock3(fs.x);
    fs.readcount++;
    if(fs.readcount == 1) {
        lock4.lock();
    }
    lock3.unlock();
    lock2.unlock();
    lock1.unlock();
    std::unique_lock<std::mutex> lock5(output);
    std::cout<<"I am reading!"<<std::endl;
    lock5.unlock();
    std::unique_lock<std::mutex> lock6(fs.x);
    fs.readcount -- ;
    if(fs.readcount == 0) {
        lock4.unlock();
    }
    lock6.unlock();
}

void writer(){
    std::unique_lock<std::mutex> lock2(fs.rsem);
    lock2.unlock();
    std::unique_lock<std::mutex> lock1(fs.y);
    fs.writecount++;
    if(fs.writecount == 1) lock2.lock();
    lock1.unlock();
    std::unique_lock<std::mutex> lock3(fs.wsem);
    std::cout<<"I am writing!!"<<std::endl;
    lock3.unlock();
    std::unique_lock<std::mutex> lock4(fs.y);
    fs.writecount--;
    if(fs.writecount == 0) lock2.unlock();
    lock4.unlock();
}

void Read_Write(){
    std::thread read1(reader);
    std::thread read2(reader);
    std::thread read3(reader);
    std::thread write1(writer);
    std::thread write2(writer);

    read1.join();
    read2.join();
    read3.join();
    write1.join();
    write2.join();
}

int main()
{
    Read_Write();
}