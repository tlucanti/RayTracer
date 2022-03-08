/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 12:09:17 by kostya            #+#    #+#             */
/*   Updated: 2022/02/06 17:43:15 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

# if __cplusplus <= 199711L
#  define __WUR __attribute__((warn_unused_result))
#  ifndef noexcept
#   define noexcept throw()
#  endif /* noexcept */
#  ifndef override
#   define override
#  endif /* override */
#  ifndef nullptr
#   define nullptr NULL
#  endif /* nullptr */
#  define __DEFAULT {}
#  define __UNUSED __attribute__((unused))
#  define __NORET __attribute__((noreturn))
#  define __DELETE ;
#  define __DELETED_MEMBERS private

# else
#  define __WUR [[nodiscard]]
#  define __DEFAULT =default;
#  define __UNUSED [[maybe_unused]]
#  define __NORET [[noreturn]]
#  define __DELETE =delete;
#  define __DELETED_MEMBERS public

# endif /* 199711L */
#endif /* DEFS_H */
