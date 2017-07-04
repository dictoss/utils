;;;;;;;;;;;;;;;;;;;
;; irchat
;;;;;;;;;;;;;;;;;;;
(setq load-path (cons (expand-file-name "~/.emacs.d/irchat") load-path))
(autoload 'irchat "irchat" "Internet Relay Chat" t)
(setq irchat-server-alist
	  '(("debianjp" . "irc.debian.or.jp"))
)
