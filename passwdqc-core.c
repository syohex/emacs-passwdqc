/*
  Copyright (C) 2016 by Syohei YOSHIDA

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emacs-module.h>
#include <passwdqc.h>

int plugin_is_GPL_compatible;

static char*
retrieve_string(emacs_env *env, emacs_value str, ptrdiff_t *size)
{
	*size = 0;

	env->copy_string_contents(env, str, NULL, size);
	char *p = malloc(*size);
	if (p == NULL) {
		*size = 0;
		return NULL;
	}
	env->copy_string_contents(env, str, p, size);

	return p;
}

static emacs_value
Fpasswdqc_generate(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	passwdqc_params_t params;
	passwdqc_params_reset(&params);

	char *passwd = passwdqc_random(&(params.qc));
	if (passwd == NULL) {
		return env->intern(env, "nil");
	}

	emacs_value retval = env->make_string(env, passwd, strlen(passwd));
	free(passwd);
	return retval;
}

static emacs_value
Fpasswdqc_check(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	passwdqc_params_t params;
	passwdqc_params_reset(&params);

	ptrdiff_t new_size, old_size;
	char *new_passwd = retrieve_string(env, args[0], &new_size);
	char *old_passwd = NULL;

	if (env->is_not_nil(env, args[1])) {
		old_passwd = retrieve_string(env, args[1], &old_size);
	}

	const char *reason = passwdqc_check(&(params.qc), new_passwd, old_passwd, NULL);
	if (reason != NULL) {
		emacs_value Qmessage = env->intern(env, "message");
		emacs_value message_args[] = {
			env->make_string(env, "%s", 2),
			env->make_string(env, reason, strlen(reason)),
		};
		env->funcall(env, Qmessage, 2, message_args);

		return env->intern(env, "nil");
	}

	if (new_passwd != NULL)
		free(new_passwd);
	if (old_passwd != NULL)
		free(old_passwd);

	return env->intern(env, "t");
}

static void
bind_function(emacs_env *env, const char *name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };

	env->funcall(env, Qfset, 2, args);
}

static void
provide(emacs_env *env, const char *feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern (env, "provide");
	emacs_value args[] = { Qfeat };

	env->funcall(env, Qprovide, 1, args);
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

#define DEFUN(lsym, csym, amin, amax, doc, data) \
	bind_function(env, lsym, env->make_function(env, amin, amax, csym, doc, data))

	DEFUN("passwdqc-generate", Fpasswdqc_generate, 0, 0, "Generate password", NULL);
	DEFUN("passwdqc--check-core", Fpasswdqc_check, 2, 2, "Check password quality", NULL);

#undef DEFUN

	provide(env, "passwdqc-core");
	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
