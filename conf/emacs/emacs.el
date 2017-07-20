;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; emacs setup 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; for debian 
; apt install org-mode python-mode
; 
; pip install pep8 flymake pychecker
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; emacs common setting
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; no startup message
(setq inhibit-startup-message t)

;; no backup files
(setq make-backup-files nil)
;; no auto-backup files
(setq auto-save-default nil)

; display row and col number.
(line-number-mode 1)
(column-number-mode 1)


; no tail line
(setq next-line-add-newlines nil)


;;;;;;;;;;;;;;;;;;
;; for japanese
;;;;;;;;;;;;;;;;;;
;; set lang
(set-language-environment "japanese")
;; input Method
;(setq default-input-method "Japanese")
;; anthy
(setq emacs-ime (getenv "anthy")) 
(setq default-input-method "japanese-anthy")
;(load-library "anthy")

;; charset, cr, lf
;(set-default-coding-systems 'euc-jp-unix) 
;(set-keyboard-coding-system 'euc-jp-unix) 
;(set-terminal-coding-system 'euc-jp-unix) 
;(set-buffer-file-coding-system 'euc-jp-unix)

(set-default-coding-systems 'utf-8-unix) 
(set-keyboard-coding-system 'utf-8-unix) 
(set-terminal-coding-system 'utf-8-unix) 
(set-buffer-file-coding-system 'utf-8-unix)


;;;;;;;;;;;;;;;;;;;;;;;;
;; keyboard config
;;;;;;;;;;;;;;;;;;;;;;;;
;; set C-h to backspace
(global-set-key "\C-h" 'delete-backward-char)

;; back delte on Delete-key
(global-set-key [delete] 'delete-char)

;; press Home-key to buffer-head
(global-set-key [home] 'beginning-of-buffer )

;; oress End-key to buffer-end
(global-set-key [end] 'end-of-buffer )

;; enable pageup-key and pagedown-key
(global-set-key [pageup] 'pager-page-up)
(global-set-key [pagedown] 'pager-page-down)

;; undo on Ctrl + z
(global-set-key "\C-z" 'undo)


;; tab indent
(setq-default indent-tabs-mode nil)
(setq-default tab-width 4)
(setq tab-stop-list
  '(4 8 12 16 20 24 28 32 36 40 44 48 52 56 60))

;(setq indent-line-function 'indent-relative-maybe)


;;;;;;;;;;;;;;;;;;;;;;;
;; color and highlight
;;;;;;;;;;;;;;;;;;;;;;;
(global-font-lock-mode t)

; ( ) highlight
(show-paren-mode)

; set background color on X Window System only.
(if window-system (progn
  (add-to-list 'default-frame-alist '(background-color . "#333366"))
  (set-face-foreground 'default "white")
))

; cursor color
(add-to-list 'default-frame-alist '(cursor-color . "white"))

(set-face-background 'region "white")

(set-face-foreground 'font-lock-comment-face "yellow")
(set-face-foreground 'font-lock-string-face  "green")
(set-face-foreground 'font-lock-keyword-face "red")
(set-face-foreground 'font-lock-function-name-face "cyan")
(set-face-bold-p 'font-lock-function-name-face t)
(set-face-foreground 'font-lock-variable-name-face "white")
(set-face-foreground 'font-lock-type-face "red")
(set-face-foreground 'font-lock-builtin-face "magenta")
(set-face-foreground 'font-lock-constant-face "red")
(set-face-foreground 'font-lock-warning-face "magenta")
(set-face-bold-p 'font-lock-warning-face nil)
(set-face-foreground 'minibuffer-prompt "yellow")

; shell
(autoload 'ansi-color-for-comint-mode-on "ansi-color"
          "Set `ansi-color-for-comint-mode' to t." t)
(add-hook 'shell-mode-hook 'ansi-color-for-comint-mode-on)


;;;;;;;;;;;;;;;;;;;;;;
;; program language
;;;;;;;;;;;;;;;;;;;;;;;

;; C and C++
(load "~/.emacs.d/emacs-c.el")

;; python
(load "~/.emacs.d/emacs-py.el")

;; php
(load "~/.emacs.d/emacs-php.el")


;;;;;;;;;;;;;;;;;;;
;; tools
;;;;;;;;;;;;;;;;;;;

;; twitter
(load "~/.emacs.d/emacs-twit.el")

;; org-mode
(load "~/.emacs.d/emacs-orgmode.el")
