/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wquinoa <wquinoa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 12:35:38 by wquinoa           #+#    #+#             */
/*   Updated: 2020/07/13 14:34:30 by wquinoa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libftprintf.h"

static void	ft_print_signed(va_list arg, t_spec *s)
{
	const int		num = va_arg(arg, long);
	const t_uint8	*base = &(s->base);
	const t_uint8	len = ft_nlen(ft_abs(num), *base);

	s->width -= (ft_max(s->precision, len) + (num < 0)) - \
	((num == 0) && (s->precision <= 0) && (s->flags & (precision2_f | dot_f)));
	s->precision -= len;
	if (!(s->flags & (minus_f | zero_f)))
		ft_pad(' ', s->width, s);
	if ((s->type == 'd' || s->type == 'i'))
		ft_putnbr_basep(num, *base, s);
	if (s->flags & minus_f)
		ft_pad(' ', s->width, s);
}

static void	ft_print_unsigned(va_list arg, t_spec *s)
{
	const t_uint64	number = va_arg(arg, unsigned long long);
	const t_uint8	*base = &(s->base);
	const t_uint8	len = ft_nlen(number, *base);
	const t_uint8	is_ptr = ((s->type == 'p'));

	s->width -= (ft_max(s->precision, len) + is_ptr * 2 - \
	(!number && (s->precision <= 0) && (s->flags & (precision2_f | dot_f))));
	s->precision -= len;
	if (!(s->flags & (minus_f | zero_f)))
		ft_pad(' ', s->width, s);
	if (s->type == 'X')
		ft_putull_basep((t_uint32)number, (*base + upper), s);
	else if (s->type == 'x')
		ft_putull_basep((t_uint32)number, *base, s);
	else if (s->type == 'p')
		ft_putull_basep(number, *base + hex, s);
	else if (s->type == 'u')
		ft_putull_basep((t_uint32)number, *base, s);
	if (s->flags & minus_f)
		ft_pad(' ', s->width, s);
}

static void	ft_print_chr(char c, t_spec *s)
{
	s->width -= 1;
	if ((s->flags & minus_f) == 0)
		ft_pad((s->flags & zero_f) ? '0' : ' ', s->width, s);
	s->length += write(1, (s->type == 'c') ? &c : "%", 1);
	if (s->flags & minus_f)
		ft_pad(' ', s->width, s);
}

static void	ft_print_str(char *arg, t_spec *s)
{
	const char		*str = (!arg) ? "(null)" : arg;
	t_uint32		len;

	if (s->flags & dot_f)
		len = ft_min(s->precision, ft_strlen(str));
	if (!(s->flags & dot_f) || (s->precision < 0))
		len = ft_strlen(str);
	s->width -= len;
	if ((s->flags & minus_f) == 0)
		ft_pad((s->flags & zero_f) ? '0' : ' ', s->width, s);
	s->length += write(1, str, len);
	if (s->flags & minus_f)
		ft_pad(' ', s->width, s);
}

int			ft_define_type(const char type, va_list arg, t_spec *specifier)
{
	const char	*is_numeric = ft_strchr(&SPECS[3], type);
	t_uint8		*base;

	if (type == '\0')
		return (0);
	if (specifier->precision || (specifier->flags & minus_f))
		specifier->flags &= ~zero_f;
	specifier->type = type;
	base = &(specifier->base);
	*base = (type == 'x' || type == 'X' || type == 'p') ? 16 : 10;
	if (is_numeric)
	{
		if (*base == 16 || type == 'u')
			ft_print_unsigned(arg, specifier);
		else if (*base == 10)
			ft_print_signed(arg, specifier);
	}
	else if (type == 's')
		ft_print_str(va_arg(arg, char *), specifier);
	else if (type == 'c' || type == '%')
		ft_print_chr((type == 'c') ? va_arg(arg, int) : '%', specifier);
	return (1);
}
