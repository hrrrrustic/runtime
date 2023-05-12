// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

namespace System.ComponentModel.DataAnnotations
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field | AttributeTargets.Parameter,
        AllowMultiple = false)]
    public sealed class UrlAttribute : DataTypeAttribute
    {
        public UrlAttribute()
            : base(DataType.Url)
        {
            // Set DefaultErrorMessage not ErrorMessage, allowing user to set
            // ErrorMessageResourceType and ErrorMessageResourceName to use localized messages.
            DefaultErrorMessage = SR.UrlAttribute_Invalid;
        }

        public override bool IsValid(object? value)
        {
            if (value is null)
                return true;

            if (value is string str)
                return IsValidString(str);

            return value is Uri uri && IsValidUri(uri);
        }

        private static bool IsValidUri(Uri uri) =>
            uri.Scheme == Uri.UriSchemeHttp
            || uri.Scheme == Uri.UriSchemeHttps
            || uri.Scheme == Uri.UriSchemeFtp;

        private static bool IsValidString(string value) =>
            value.StartsWith("http://", StringComparison.OrdinalIgnoreCase)
            || value.StartsWith("https://", StringComparison.OrdinalIgnoreCase)
            || value.StartsWith("ftp://", StringComparison.OrdinalIgnoreCase);
    }
}
