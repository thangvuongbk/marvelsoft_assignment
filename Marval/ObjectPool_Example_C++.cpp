
#include <iostream>
#include <string>
#include <map>

using namespace std;

class Resource
{
    public:
        Resource()
        {
            cout << "Resource is created -> " << ++object_idx << endl;
        }
    private:
     static int object_idx;

};

int Resource::object_idx = 0;

/* Note, that this class is a singleton. */
class ObjectPool
{
    private:
        std::map<std::string, Resource*> resources;

        //static ObjectPool* instance;
        ObjectPool() {}
    public:
        /**
         * Static method for accessing class instance.
         * Part of Singleton design pattern.
         *
         * @return ObjectPool instance.
         */
        static ObjectPool* getInstance()
        {
            static ObjectPool instance;
            return &instance;
        }
        /**
         * Returns instance of Resource.
         *
         * New resource will be created if all the resources
         * were used at the time of the request.
         *
         * @return Resource instance.
         */
        Resource* getResource(std::string objectName)
        {
            auto resource = resources.find(objectName);
            if (resource == resources.end())
            {
                std::cout << "Creating new." << std::endl;
                Resource* newObject = new Resource();
                resources.insert(std::pair<std::string, Resource*> (objectName, newObject));
                return newObject;
            }
            else
            {
                std::cout << "Reusing existing." << std::endl;
                return resource->second;
            }
        }
};

int main()
{
    ObjectPool* getIns = ObjectPool::getInstance();
    Resource* objRes1 = getIns->getResource("CMNI");
    Resource* objRes2 = getIns->getResource("CMNI");

    return 0;
}