/*
** mlx_int_param_event.c for MinilibX in 
** 
** Made by Charlie Root
** Login   <ol@epitech.net>
** 
** Started on  Mon Jul 31 16:37:50 2000 Charlie Root
** Last update Wed Oct  6 13:14:52 2004 Olivier Crouzet
*/

#include	"mlx_int.h"

int	mlx_int_param_undef()
{
    return 0;
}

int	mlx_int_param_KeyPress(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[KeyPress].hook(XkbKeycodeToKeysym(xvar->display,
					       ev->xkey.keycode, 0, 0),
			    win->hooks[KeyPress].param);
  return 0;
}

int	mlx_int_param_KeyRelease(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[KeyRelease].hook(XkbKeycodeToKeysym(xvar->display,
						 ev->xkey.keycode, 0, 0),
			      win->hooks[KeyRelease].param);
  return 0;
}

int	mlx_int_param_ButtonPress(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[ButtonPress].hook(ev->xbutton.button,ev->xbutton.x,ev->xbutton.y,
			       win->hooks[ButtonPress].param);
  return 0;
}

int	mlx_int_param_ButtonRelease(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[ButtonRelease].hook(ev->xbutton.button,
				 ev->xbutton.x, ev->xbutton.y,
				 win->hooks[ButtonRelease].param);
  return 0;
}

int	mlx_int_param_MotionNotify(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[MotionNotify].hook(ev->xbutton.x,ev->xbutton.y,
				win->hooks[MotionNotify].param);
  return 0;
}

int	mlx_int_param_Expose(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  if (!ev->xexpose.count)
    win->hooks[Expose].hook(win->hooks[Expose].param);
  return 0;
}


int	mlx_int_param_generic(__attribute__((unused)) t_xvar *xvar, XEvent *ev, t_win_list *win)
{
  win->hooks[ev->type].hook(win->hooks[ev->type].param);
  return 0;
}

int	(*mlx_int_param_event[])(t_xvar *, XEvent *, t_win_list *) =
{
  mlx_int_param_undef,   /* 0 */
  mlx_int_param_undef,
  mlx_int_param_KeyPress,
  mlx_int_param_KeyRelease,  /* 3 */
  mlx_int_param_ButtonPress,
  mlx_int_param_ButtonRelease,
  mlx_int_param_MotionNotify,  /* 6 */
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_Expose,   /* 12 */
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic,
  mlx_int_param_generic
};
