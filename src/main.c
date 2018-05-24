/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 13:45:48 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/24 16:36:17 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <inttypes.h>

char	*get_symbol_type(uint8_t type)
{
	if (type == N_UNDF)
		return ("U");
	else if (type == N_ABS)
		return ("A");
	else if (type == N_SECT)
		return ("t");
	else if (type == N_PBUD)
		return ("u");
	else if (type == N_INDR)
		return ("I");
	else if (type == N_STAB)
		return ("D");
	else if (type == N_PEXT)
		return ("E");
	else if (type == N_TYPE)
		return ("T");
	else if (type == N_EXT)
		return ("U");
	return (" ");
}

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**tab;
	char			**tabvalues;
	char			**tabtype;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;
	tab = (char**)malloc(sizeof(char*) * nsyms);
	tabvalues = (char**)malloc(sizeof(char*) * nsyms);
	tabtype = (char**)malloc(sizeof(char*) * nsyms);
	i = -1;
	while (++i < nsyms)
	{
		tab[i] = ft_addrtochar(array[i].n_value);
		tabtype[i] = get_symbol_type(array[i].n_type);
		tabvalues[i] = stringtable + array[i].n_un.n_strx;
	}
	ft_sorttabtwofollow(&tabvalues, &tab, &tabtype, nsyms);
	i = -1;
	while (++i < nsyms)
	{
		ft_putstr(tab[i]);
		ft_putchar(' ');
		ft_putstr(tabtype[i]);
		ft_putchar(' ');
		ft_putstr(tabvalues[i]);
		ft_putchar('\n');
	}
	free(tab);
	free(tabvalues);
}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64*)ptr;
	ncmds = header->ncmds;
	i = -1;
	lc = (void*)ptr + sizeof(*header);
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
}

void	nm(char *ptr)
{
	int		magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
}

int		main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac != 2)
		return (EXIT_FAILURE);
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
