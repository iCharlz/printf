#include "main.h"

/************************* WRITE HANDLE *************************/
/**
 * handle_write_char - Prints a character
 * @c: The char to be printed
 * @buffer: Buffer array for handling print
 * @flags: Flag calculation
 * @width: specifier for width
 * @precision: specifier precision
 * @size: Size specifier
 *
 * Return: Number of characters printed
 */
int handle_write_char(char c, char buffer[],
	int flags, int width, int precision, int size)
{
	int i = 0;
	char padd = ' ';

	UNUSED(precision);
	UNUSED(size);

	if (flags & F_ZERO)
	padd = '0';

	buffer[i++] = c;
	buffer[i] = '\0';

	if (width > 1)
	{
	buffer[BUFF_SIZE - 1] = '\0';
	for (i = 0; i < width - 1; i++)
	buffer[BUFF_SIZE - i - 2] = padd;

	if (flags & F_MINUS)
	return (write(1, &buffer[0], 1) +
	write(1, &buffer[BUFF_SIZE - i - 1], width - 1));
	else
	return (write(1, &buffer[BUFF_SIZE - i - 1], width - 1) +
	write(1, &buffer[0], 1));
	}

	return (write(1, &buffer[0], 1));
}

/************************* WRITE NUMBER *************************/
/**
 * write_number - Prints a number
 * @is_negative: checks if the number is negative
 * @ind: Index of the number in the buffer
 * @buffer: Buffer array for handling print
 * @flags: Flag calculation
 * @width: specifier for width
 * @precision: specifier for precision
 * @size: specifier for size
 *
 * Return: Number of characters printed
 */
int write_number(int is_negative, int ind, char buffer[],
	int flags, int width, int precision, int size)
{
	int length = BUFF_SIZE - ind - 1;
	char padd = ' ', extra_ch = 0;

	UNUSED(size);

	if ((flags & F_ZERO) && !(flags & F_MINUS))
	padd = '0';
	if (is_negative)
	extra_ch = '-';
	else if (flags & F_PLUS)
	extra_ch = '+';
	else if (flags & F_SPACE)
	extra_ch = ' ';

	return (write_num(ind, buffer, flags, width, precision,
	length, padd, extra_ch));
}

/**
 * write_num - Writes a number using a buffer
 * @ind: Index of the number in the buffer
 * @buffer: Buffer array for handling print
 * @flags: Flag calculation
 * @width:  specifier for width
 * @prec: specifier for precision
 * @length: Length of the number
 * @padd: Padding character
 * @extra_c: Extra character
 *
 * Return: Number of characters printed
 */
int write_num(int ind, char buffer[],
	int flags, int width, int prec,
	int length, char padd, char extra_c)
{
	int i, padd_start = 1;

	if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0' && width == 0)
	return (0); /* printf(".0d", 0) no character is printed */
	if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
	buffer[ind] = padd = ' '; /* Display width with padding ' ' */
	if (prec > 0 && prec < length)
	padd = ' ';
	while (prec > length)
	buffer[--ind] = '0', length++;
	if (extra_c != 0)
	length++;
	if (width > length)
	{
	for (i = 1; i < width - length + 1; i++)
	buffer[i] = padd;
	buffer[i] = '\0';
	if (flags & F_MINUS && padd == ' ')
	{
	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length) + write(1, &buffer[1], i - 1));
	}
	else if (!(flags & F_MINUS) && padd == ' ')
	{
	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[1], i - 1) + write(1, &buffer[ind], length));
	}
	else if (!(flags & F_MINUS) && padd == '0')
	{
	if (extra_c)
	buffer[--padd_start] = extra_c;
	return (write(1, &buffer[padd_start], i - padd_start) +
	write(1, &buffer[ind], length - (1 - padd_start)));
	}
	}
	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length));
}

/**
 * write_unsgnd - Writes an unsigned number
 * @is_negative: Number checking if the number is negative
 * @ind: Index of the number in the buffer
 * @buffer: Buffer array for handling print
 * @flags: Flag calculation
 * @width: specifier for width
 * @precision: specifier for precision
 * @size: Size specifier
 *
 * Return: Number of characters printed
 */
int write_unsgnd(int is_negative, int ind,
	char buffer[],
	int flags, int width, int precision, int size)
{
	int length = BUFF_SIZE - ind - 1, i = 0;
	char padd = ' ';

	UNUSED(is_negative);
	UNUSED(size);

	if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
	return (0); /* printf(".0d", 0) no character is printed */

	if (precision > 0 && precision < length)
	padd = ' ';

	while (precision > length)
	{
	buffer[--ind] = '0';
	length++;
	}

	if ((flags & F_ZERO) && !(flags & F_MINUS))
	padd = '0';

	if (width > length)
	{
	for (i = 0; i < width - length; i++)
	buffer[i] = padd;

	buffer[i] = '\0';

	if (flags & F_MINUS)
	{
	return (write(1, &buffer[ind], length) + write(1, &buffer[0], i));
	}
	else
	{
	return (write(1, &buffer[0], i) + write(1, &buffer[ind], length));
	}
	}

	return (write(1, &buffer[ind], length));
}

/**
 * write_pointer - Writes a memory address
 * @buffer: Buffer array for handling print
 * @ind: Index of the number in the buffer
 * @length: Length of the number
 * @width: specifier for width
 * @flags: Flag calculation
 * @padd: Padding character
 * @extra_c: Extra character
 * @padd_start: Index at which padding should start
 *
 * Return: Number of characters printed
 */
int write_pointer(char buffer[], int ind, int length,
	int width, int flags, char padd, char extra_c, int padd_start)
{
	int i = 0;

	if (width > length)
	{
	for (i = 0; i < width - length; i++)
	buffer[i] = padd;

	buffer[i] = '\0';

	if (flags & F_MINUS)
	{
	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length) + write(1, &buffer[0], i));
	}
	else if (!(flags & F_MINUS) && padd == ' ')
	{
	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[0], i) + write(1, &buffer[ind], length));
	}
	else if (!(flags & F_MINUS) && padd == '0')
	{
	if (extra_c)
	buffer[--padd_start] = extra_c;
	return (write(1, &buffer[padd_start], i - padd_start) +
	write(1, &buffer[ind], length - (1 - padd_start)));
	}
	}

	if (extra_c)
	buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length));
}
