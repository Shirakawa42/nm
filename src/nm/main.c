/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 13:34:59 by lvasseur          #+#    #+#             */
/*   Updated: 2019/11/13 18:34:27 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

char	*get_symbol_type(uint8_t type)
{
	if (type == N_UNDF)
		return (" U ");
	else if (type == N_ABS)
		return (" A ");
	else if (type == N_SECT)
		return (" d ");
	else if (type == N_PBUD)
		return (" u ");
	else if (type == N_INDR)
		return (" I ");
	else if (type == N_STAB)
		return (" D ");
	else if (type == N_PEXT)
		return (" E ");
	else if (type == N_TYPE)
		return (" S ");
	else if (type == N_EXT)
		return (" U ");
	else if (type == 0xF)
		return (" T ");
	return ("   ");
}

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**tab;
	char			**tab_values;
	char			**tab_type;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;
	tab = (char**)malloc(sizeof(char*) * nsyms);
	tab_values = (char**)malloc(sizeof(char*) * nsyms);
	tab_type = (char**)malloc(sizeof(char*) * nsyms);
	i = 0;
	while (i < nsyms)
	{
		tab_values[i] = ft_addrtochar(array[i].n_value);
		tab[i] = ft_strdup(stringtable + array[i].n_un.n_strx);
		tab_type[i] = ft_strdup(get_symbol_type(array[i].n_type & N_TYPE));
		i++;
	}
	ft_sorttabtwofollow(&tab, &tab_values, &tab_type, nsyms);
	i = 0;
	while (i < nsyms)
	{
		ft_putstr(tab_values[i]);
		ft_putstr(tab_type[i]);
		ft_putendl(tab[i]);
		i++;
	}
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
	lc = (void*)ptr + sizeof(*header);
	for (i = 0; i < ncmds; ++i)
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
	int	magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == (int)MH_MAGIC_64)
		handle_64(ptr);
}

int		main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac != 2)
	{
		fprintf(stderr, "Please give me an arg\n");
		return (EXIT_FAILURE);
	}
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
	return (0);
}
