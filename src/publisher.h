#ifndef PUBLISHER_H
#define PUBLISHER_H

class Publisher
{
public:
    Publisher();
    void Publish(std::string topic, std::string content);
};

#endif // PUBLISHER_H