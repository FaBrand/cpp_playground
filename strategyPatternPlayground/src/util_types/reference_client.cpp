#include "reference_client.h"

void ReferenceClient::ExecuteStrategy()
{
    current_strategy_.get().DoWork();
}
