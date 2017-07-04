;;;;;;;;;;;;;;;;;;;
;; org-mode
;;;;;;;;;;;;;;;;;;;
;(add-to-list 'load-path "~/.emacs.d/site-lisp/org/lisp")

(require 'org-install)
;; org-mode key-bind
(define-key global-map "\C-cl" 'org-store-link)
(define-key global-map "\C-ca" 'org-agenda)
(define-key global-map "\C-cr" 'org-remember)
;; if *.org file,  auto start org-mode.
(add-to-list 'auto-mode-alist '("\\.org$" . org-mode))
;; high-light
(add-hook 'org-mode-hook 'turn-on-font-lock)
(setq org-hide-leading-stars t)

(setq org-directory "~/org/")
(setq org-mobile-inbox-for-pull "~/org/flagged.org")
;; Set to <your Dropbox root directory>/MobileOrg.
; (setq org-mobile-directory "~/Dropbox/MobileOrg")
(setq org-mobile-directory "/mnt/win-dropbox/MobileOrg")
(setq org-default-notes-file "notes.org")

(setq org-agenda-files
      '(
        "~/org/todo.org"
        "~/org/notes.org"
        "~/org/idea.org"
        "~/org/article.org"
        "~/org/buy.org"))

;; TODO mode
(setq org-todo-keywords
      '((sequence "TODO(t)" "WAIT(w)" "|" "DONE(d)" "SOMEDAY(s)")))
;; mark DONE time
(setq org-log-done 'time)
;; for org-remember
;(org-remember-insinuate)
;; org-remember's template
(setq org-remember-templates
      '(("Note" ?n "* %?\n  %i\n  %a" nil "Tasks")
		("Todo" ?t "* TODO %?\n  %i\n  %a" nil "Tasks")))
