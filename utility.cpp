#include "utility.h"

bool isDouble(const std::string& str)
{
    bool result = true;
    try
    {
        std::stod(str);
    }
    catch (const std::invalid_argument& e)
    {
        result = false;
    }
    catch (const std::out_of_range& e)
    {
        result = false;
    }

    return result;
}
