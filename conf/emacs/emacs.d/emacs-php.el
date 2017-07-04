;;;;;;;;;;;;;;;;;;;;;
;; PHP
;;;;;;;;;;;;;;;;;;;;;
;(require 'php-mode)
(autoload 'php-mode "php-mode" "Major mode for editing php code." t)
(add-hook 'php-mode-user-hook
		  '(lamda ()
		   (setq tab-width 4)
           (setq indent-tabs-mode t)))
