;;; passwdqc.el --- libpasswdqc binding of Emacs Lisp -*- lexical-binding: t; -*-

;; Copyright (C) 2016 by Syohei YOSHIDA

;; Author: Syohei YOSHIDA <syohex@gmail.com>
;; URL: https://github.com/syohex/
;; Version: 0.01
;; Package-Requires: ((emacs "25"))

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:

;; libpasswdqc(http://www.openwall.com/passwdqc/) binding of Emacs Lisp.
;; This package uses dynamic module feature.

;;; Code:

(require 'passwdqc-core)
(require 'cl-lib)

(defun passwdqc-check (new-password &optional old-password)
  (cl-assert (stringp new-password))
  (passwdqc--check-core new-password old-password))

(provide 'passwdqc)

;;; passwdqc.el ends here
