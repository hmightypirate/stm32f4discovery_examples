#include "button.h"

/**
 * @brief Function to read user button.
 */
bool button_read_user(void)
{
	return (bool)(gpio_get(GPIOA, GPIO0));
}