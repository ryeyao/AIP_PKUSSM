/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */
#include "Packet.h"

bool Packet::isFull() 
{ 
    return came == 3; 
}

Packet::Packet()
{
    Packet(0);
}
Packet::Packet(int t) :ip(""), time(0), ttl(t), came(0) {}

void Packet::setIP(string ipaddress)
{
    if(ip.empty())
    {
        ip = ipaddress;
    }
    else if( ip != ipaddress)
    {
        ip += " " + ipaddress;
    }
}
void Packet::signNew(double usec)
{
    came++;
    time += usec;
}
void Packet::print()
{
    printf("%d. %s ",ttl,ip.c_str());
    if(isFull())
        printf("%4.0lfms",(time/3));
    else
        printf("???");
    printf("\n");
}
bool Packet::isFilled()
{
  return came > 0;
}
